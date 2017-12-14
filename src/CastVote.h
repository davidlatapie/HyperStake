//
// Created by carrie on 12/12/17.
//

#ifndef HYPERSTAKE_CASTVOTE_H
#define HYPERSTAKE_CASTVOTE_H

class CCastVote {
private:
    uint32_t nVersion;
    CVoteProposal voteProposal;
public:
    CCastVote(uint32_t nVersion, CVoteProposal voteProposal) {
        this->nVersion = nVersion;
        this->voteProposal = voteProposal;
    }
};

#endif //HYPERSTAKE_CASTVOTE_H
