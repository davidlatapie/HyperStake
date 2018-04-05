// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "voteproposalmanager.h"
#include "voteproposal.h"

using namespace std;

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
        if (newProposal.location.first < existingProposal.location.second)
            continue;
        //Clear of any conflicts, ends before the existing proposal starts
        if (newProposal.location.second > existingProposal.location.first)
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

bool CVoteProposalManager::GetNextLocation(int nBitCount, int nStartHeight, int nCheckSpan, VoteLocation& location)
{
    vector<CProposalMetaData> vConflictingTime;
    for (auto it : mapProposalData) {
        CProposalMetaData data = it.second;
        int nEndHeight = nStartHeight + nCheckSpan;
        if (data.nHeightEnd < nStartHeight)
            continue;
        if (data.nHeightStart > nEndHeight)
            continue;
        vConflictingTime.emplace_back(data);
    }

    //Find an open location for the new proposal, return left most bits
    if (vConflictingTime.empty()) {
        location.first = 28;
        location.second = location.first - nBitCount;
        return true;
    }

    //create a vector tracking available spots
    vector<int> vAvailable;
    for (int i = 28; i > 0; i--)
        vAvailable.emplace_back(i);

    //remove spots that are already taken
    for (auto data : vConflictingTime) {
        int n = data.location.first;
        while (n >= data.location.second) {
            auto it = std::find(vAvailable.begin(), vAvailable.end(), n);
            if (it != vAvailable.end())
                vAvailable.erase(it);
            n--;
        }
    }

    //find an available sequence of bits that fit the proposal
    int nPrev = 0;
    int nSequenceStart = 0;
    for (auto n : vAvailable) {
        //See if the previous is in sequence
        if (nPrev) {
            if (n - 1 == nPrev)
                nSequenceStart = n;
            else
                nSequenceStart = 0;

            if (n - nSequenceStart >= nBitCount) {
                location.first = nSequenceStart;
                location.second = nSequenceStart - nBitCount;
                return true;
            }
        }
        nPrev = n;
    }
    return false;
}
