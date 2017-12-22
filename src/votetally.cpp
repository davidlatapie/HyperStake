#include "votetally.h"

using namespace std;


// takes uint32_t of the vote
// if the vote is yes, adds to YesTally
// returns YesTally after update, if any
int CVoteTally::CountVote(uint32_t voteFromVersion) {
    cout << "voteFromVersion: " << PrintBinary(voteFromVersion) << endl;

    if (nBlocksCounted < proposal.GetCheckSpan()) {
        cout << "blocks counted: " << nBlocksCounted << endl;
        if (voteFromVersion == 1)
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
    uint32_t nVoteFromVersion = voteObject.GetVoteFromVersion(nVersion);
    return CountVote(nVoteFromVersion);
}

int CVoteTally::GetYesVotes()
{
    cout << "yes tally with print binary: " << PrintBinary(nYesTally) << endl;
    return nYesTally;
}


