// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
