// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef HYPERSTAKE_VOTETALLY_H
#define HYPERSTAKE_VOTETALLY_H

#include "voteproposal.h"
#include <map>

class CVoteTally {
private:
    int nBlocksCounted;
    std::map<int, int> mapVotes;
    CVoteProposal proposal;
public:
    CVoteTally(CVoteProposal proposal)

    {
        this->proposal = proposal;
        this->nBlocksCounted = 0;
    }

};
#endif //HYPERSTAKE_VOTETALLY_H
