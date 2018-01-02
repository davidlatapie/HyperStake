#ifndef VOTING_VOTETALLY_H
#define VOTING_VOTETALLY_H

#include <map>
#include "voteproposal.h"

class CVoteTally {
private:
    std::map<int, int> mapVotes;
    int nYesTally; // store the total yes's so far
    int nNoTally;
    int nBlocksCounted = 0; // store the total number of blocks checked so far
    CVoteProposal proposal;
public:
    CVoteTally(CVoteProposal proposal)
    {
        this->proposal = proposal;
    }

    void SetNoTally() {nNoTally = nBlocksCounted - nYesTally;}
    int CountVote(uint32_t voteFromVersion);
    int ProcessVersion(const uint32_t& nVersion);
    int GetYesVotes() {return nYesTally;};
    bool AddVoteToVersion(uint32_t& nVersion);
    std::string toString();
};

#endif //VOTING_VOTETALLY_H

