// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "voteproposalmanager.h"
#include "voteproposal.h"

using namespace std;

bool CVoteProposalManager::CheckProposal(const CVoteProposal &proposal)
{
    // If the proposal is already in the blockchain then it's guaranteed to be valid
    if (mapProposalData.count(proposal.GetHash()) != 0) {
        return true;
    }

    // Proposal name length must be between 1 and MAX_CHAR_NAME (inclusive)
    if (proposal.GetName().empty() || proposal.GetName().size() > MAX_CHAR_NAME) {
        return false;
    }

    // Proposal description length must be between 1 and MAX_CHAR_ABSTRACT (inclusive)
    if (proposal.GetDescription().empty() || proposal.GetDescription().size() > MAX_CHAR_ABSTRACT) {
        return false;
    }

    // Proposal voting period cannot start before or at the current height or after MAX_BLOCKS_IN_FUTURE
    if (proposal.GetStartHeight() <= nBestHeight || proposal.GetStartHeight() > nBestHeight + MAX_BLOCKS_IN_FUTURE) {
        return false;
    }

    // Proposal voting period length must be between 1 and MAX_CHECKSPAN (inclusive)
    if (!proposal.GetCheckSpan() || proposal.GetCheckSpan() > MAX_CHECKSPAN) {
        return false;
    }

    // Check to see if there is room on the blockchain for this proposal
    VoteLocation location;
    if (!GetNextLocation(proposal.GetBitCount(), proposal.GetStartHeight(), proposal.GetCheckSpan(), location)) {
        return false;
    }

    return true;
}

//! Add a proposal to the manager. Note that it must not have conflicts in its scheduling.
bool CVoteProposalManager::Add(const CVoteProposal& proposal)
{
    CProposalMetaData newProposal;
    newProposal.location = proposal.GetLocation();
    newProposal.hash = proposal.GetHash();
    newProposal.nHeightStart = proposal.GetStartHeight();
    newProposal.nHeightEnd = newProposal.nHeightStart + proposal.GetCheckSpan();

    //Check if any of the existing proposals are using the same bits during the same time
    for (auto it : mapProposalData) {
        CProposalMetaData existingProposal = it.second;
        //Clear of any conflicts, starts after the existing proposal ends
        if (newProposal.location.nMostSignificantBit < existingProposal.location.nLeastSignificantBit)
            continue;
        //Clear of any conflicts, ends before the existing proposal starts
        if (newProposal.location.nLeastSignificantBit > existingProposal.location.nMostSignificantBit)
            continue;
        //Clear of any conflicts, there is not overlap in the voting period
        if (newProposal.nHeightStart > existingProposal.nHeightEnd || newProposal.nHeightEnd < existingProposal.nHeightStart)
            continue;

        return error("%s: Proposal position is already occupied during the block span requested", __func__);
    }

    mapProposalData.insert(make_pair(newProposal.hash, newProposal));
    printf("%s: added proposal %s\n", __func__, newProposal.hash.GetHex().c_str());
    return true;
}

//! Remove a proposal from the proposal manager
void CVoteProposalManager::Remove(const uint256& hashProposal)
{
    auto it = mapProposalData.find(hashProposal);
    if (it != mapProposalData.end())
        mapProposalData.erase(it);
}

//! Get proposals that are actively being voted on
map<uint256, VoteLocation> CVoteProposalManager::GetActive(int nHeight)
{
    map<uint256, VoteLocation> mapActive;
    for (auto it : mapProposalData) {
        CProposalMetaData data = it.second;
        if ((int)data.nHeightStart <= nHeight && (int)data.nHeightEnd >= nHeight)
            mapActive.insert(make_pair(data.hash, data.location));
    }

    return mapActive;
}

namespace
{
    //An Event is either the beginning or end of a vote proposal span.
    //This struct is used for GetMaxOverlap
    struct Event
    {
        bool start;
        int position;
        int bitCount;

        //default constructor
        Event() {}

        Event(bool start, int position, int bitCount = 0)
        {
            this->start = start;
            this->position = position;
            this->bitCount = bitCount;
        }

        static bool Compare(const Event& lhs, const Event& rhs)
        {
            return lhs.position < rhs.position;
        }
    };

    //returns the maximum number of proposals overlapping at any point within the given range
    unsigned int GetMaxOverlap(const vector<CProposalMetaData>& vProposals, const unsigned int& nStart, const unsigned int& nEnd)
    {
        int nMaxOverlapQuantity = -1;
        vector<Event> vEvents(2 * vProposals.size());
        for(auto proposalData: vProposals) {
            if(proposalData.nHeightEnd < nStart) continue;
            if(proposalData.nHeightStart > nEnd) continue;
            vEvents.emplace_back(Event(true, proposalData.nHeightStart));
            vEvents.emplace_back(Event(false, proposalData.nHeightEnd));
        }

        // sort the events in order of time (defined by Compare method in Event struct
        sort(vEvents.begin(), vEvents.end(), Event::Compare);

        // iterate through all events and keep track of overlapping intervals
        int nCurValueCounter = 0;
        for(Event event: vEvents) {
            nCurValueCounter += event.start ? 1 : -1;
            nMaxOverlapQuantity = max(nMaxOverlapQuantity, nCurValueCounter);
        }

        // return the maximum number of overlapping intervals contained in vProposals at any point in time
        return (unsigned int)nMaxOverlapQuantity;
    }

    // return a deterministic 64 bit integer that represents the resource usage of a given proposal
    long GetResourceUsageHeuristic(const vector<CProposalMetaData>& vProposals, const CVoteProposal& proposal)
    {
        long nHeuristic = 0;
        int nStart = proposal.GetStartHeight();
        int nEnd = proposal.GetStartHeight() + proposal.GetCheckSpan();

        // An event is defined as either a proposal interval start or a proposal interval ending.
        vector<Event> vEvents(2 * vProposals.size());

        // For each proposal in vProposals that overlaps with the given proposal, create a start and end event then
        // add it to vEvents. This vector will be used to determine the number of overlapping voting intervals efficiently.
        for(auto proposalData: vProposals) {
            if(proposalData.nHeightEnd < nStart) continue;
            if(proposalData.nHeightStart > nEnd) continue;

            Event startEvent(true, proposalData.nHeightStart, proposalData.location.GetBitCount());
            Event endEvent(false, proposalData.nHeightEnd + 1, proposalData.location.GetBitCount());

            vEvents.emplace_back(startEvent);
            vEvents.emplace_back(endEvent);
        }

        // sort the events so that those that happen earlier appear first in the vector
        sort(vEvents.begin(), vEvents.end(), Event::Compare);

        // iterate through events in sorted order and keep a running counter of how many bits are consumed
        int nCurValueCounter = 0;
        for(unsigned int i = 0; i < vEvents.size() - 1; i++) {
            Event curEvent = vEvents.at(i);
            Event nextEvent = vEvents.at(i + 1);

            nCurValueCounter += curEvent.start ? curEvent.bitCount : -1 * curEvent.bitCount;

            // only start the counter when we have entered the voting intervals of the given proposal
            if(nextEvent.position <= nStart) continue;
            if(curEvent.position > nEnd) break;

            // the number of bits used is guaranteed to be constant for every block between these two events
            int gap = min(nEnd, nextEvent.position) - max(nStart, curEvent.position);

            // TODO: heuristic updated; this is subject to change
            nHeuristic += (100000 * ((long) proposal.GetBitCount())) / (28 - nCurValueCounter) * gap;
        }

        return nHeuristic;
    }

    //returns a vector of proposals that overlap with the given range
    vector<CProposalMetaData> GetOverlappingProposals(const map<uint256, CProposalMetaData>& mapProposalData,
                                                      const int& nStart, const int& nEnd)
    {
        vector<CProposalMetaData> vConflictingTime;
        for (auto it : mapProposalData) {
            CProposalMetaData data = it.second;
            if ((int)data.nHeightEnd < nStart)
                continue;
            if ((int)data.nHeightStart > nEnd)
                continue;
            vConflictingTime.emplace_back(data);
        }

        return vConflictingTime;
    }
}

bool CVoteProposalManager::GetNextLocation(int nBitCount, int nStartHeight, int nCheckSpan, VoteLocation& location)
{
    //Conflicts for block range
    vector<CProposalMetaData> vConflictingTime;
    for (auto it : mapProposalData) {
        CProposalMetaData data = it.second;
        int nEndHeight = nStartHeight + nCheckSpan;
        if ((int)data.nHeightEnd < nStartHeight)
            continue;
        if ((int)data.nHeightStart > nEndHeight)
            continue;
        vConflictingTime.emplace_back(data);
    }

    //Find an open location for the new proposal, return left most bits
    if (vConflictingTime.empty()) {
        location.nMostSignificantBit = 27;
        location.nLeastSignificantBit = (uint8_t)(location.nMostSignificantBit - nBitCount + 1);
        return true;
    }

    //create a vector tracking available spots
    vector<int> vAvailable(28, 1);

    //remove spots that are already taken
    for (auto data : vConflictingTime) {
        for (int i = data.location.nMostSignificantBit; i >= data.location.nLeastSignificantBit; i--) {
            vAvailable.at(i) = 0;
        }
    }

    //find an available sequence of bits that fit the proposal
    vector<int> vRange;
    int nSequential = 0;
    for (int i = 27; i >= 0; i--) {
        uint8_t n = static_cast<uint8_t>(i);
        nSequential = vAvailable.at(n) == 1 ? nSequential + 1 : 0;
        if (nSequential == nBitCount) {
            location.nLeastSignificantBit = n;
            location.nMostSignificantBit = static_cast<uint8_t>(n + nBitCount - 1);
            return true;
        }
    }
    return false;
}