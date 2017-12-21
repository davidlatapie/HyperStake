// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef HYPERSTAKE_VOTEOBJECT_H
#define HYPERSTAKE_VOTEOBJECT_H
#define HEADERMASK 0x0fffffff

#include "voteproposal.h"

class CVoteObject
{
private:
    int nChoice;
    uint32_t nFormattedVote;
    bool fVoted;
    CVoteProposal proposal;
public:
    CVoteObject(CVoteProposal proposal, int nChoice)
    {
        this->proposal= proposal;
        this->nChoice = nChoice;
    }
    bool AddVoteToVersion(uint32_t& nVersion);
    uint32_t GetVoteFromVersion(uint32_t nVersion);
};

#endif //HYPERSTAKE_VOTEOBJECT_H
