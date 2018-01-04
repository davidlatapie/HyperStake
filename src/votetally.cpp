#include "votetally.h"


using namespace std;


// takes uint32_t of the vote
// if the vote is yes, adds to YesTally
// returns YesTally after update, if any
int CVoteTally::CountVote(uint32_t voteFromVersion, CVoteObject voteobject) {
    voteFromVersion >>= proposal.GetShift();
    cout << "voteFromVersion:            " << voteobject.PrintBinary(voteFromVersion) << endl;

    if (nBlocksCounted <= proposal.GetCheckSpan()) {
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

int CVoteTally::ProcessVersion(const uint32_t nVersion, CVoteObject voteobject)
{
    uint32_t nVoteFromVersion = voteobject.GetVoteFromVersion(nVersion);
    return CountVote(nVoteFromVersion, voteobject);
}


