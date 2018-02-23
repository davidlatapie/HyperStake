#include "votetally.h"
#include "util.h"

#define VOTEMASK 0x0FFFFFFF

using namespace std;

CVoteTally::CVoteTally(CVoteTally* tallyPrev)
{
    this->nHeight = tallyPrev->nHeight + 1;
    this->mapVotes = tallyPrev->GetVotes();
    RemoveStaleSummaries();
}

void CVoteTally::RemoveStaleSummaries()
{
    for (auto it : mapVotes) {
        //If the vote has ended, then remove it from the new Tally
        if (it.second.nBlockStart + it.second.nCheckSpan <= this->nHeight) {
            mapLocations.erase(it.first);
            mapVotes.erase(it.first);
        }
    }
}

//! Proposal Manager will give a set of new positions if any start this block
bool CVoteTally::SetNewPositions(std::map<uint256, VoteLocation> &mapNewLocations)
{
    mapLocations.clear();
    for (auto it : mapNewLocations) {
        VoteLocation locationNew = it.second;

        mapLocations.insert(make_pair(it.first, locationNew));
        CVoteSummary summary;
        mapVotes.insert(make_pair(it.first, summary));
    }

    return true;
}

//! Record votes that were in the block header
void CVoteTally::ProcessNewVotes(const uint32_t& nVersion)
{
    for (auto it : mapVotes) {
        VoteLocation location = mapLocations.at(it.first);
        int32_t nVote = nVersion;
        nVote &= VOTEMASK; // remove version bits
        nVote >>= location.first + 1; //shift it over to the starting position
        int32_t nBits = location.second - location.first;

        // Remove any bits to the left of the vote bits
        int32_t nMask = 1;
        for (int i = 0; i < nBits - 1; i++) {
            nMask <<= 1;
            nMask |= 1;
        }
        nVote &= nMask;

        //Count the vote if it is yes or no
        if (nVote == 1) {
            mapVotes.at(it.first).nYesTally++;
        } else if (nVote > 1)
            mapVotes.at(it.first).nNoTally++;
    }
}

bool CVoteTally::GetSummary(const uint256& hashProposal, CVoteSummary& summary)
{
    auto it = mapVotes.find(hashProposal);
    if (it == mapVotes.end())
        return false;

    summary = it->second;

    return true;
}



