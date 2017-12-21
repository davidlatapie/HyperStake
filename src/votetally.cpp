#include "votetally.h"

using namespace std;


// takes uint32_t of the vote
// if the vote is yes, adds to YesTally
// returns YesTally after update, if any
int CVoteTally::CountVote(uint32_t voteFromVersion) {
    cout << "voteFromHeader : " << PrintBinary(voteFromVersion) << endl;

    if (GetBlocksCounted() < proposal.GetCheckSpan()) {
        cout << "getblockscounted(): " << GetBlocksCounted() << endl;
        if (voteFromHeader)
            nYesTally++;

        nBlocksCounted++;
        SetNoTally();
        return nYesTally;
    }

    return 0;
}
//Look for vote in a block version, add the vote to the tally

int CVoteTally::ProcessVersion(const uint32_t& nVersion)
{
    uint32_t nVoteFromHeader = voteObject.GetVoteFromHeader(nVersion);
    return CountVote(nVoteFromHeader);
}

int CVoteTally::GetVotes()
{
    return nYesTally;
}


