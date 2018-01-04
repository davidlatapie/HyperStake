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
    CVoteObject(CVoteProposal proposal)
    {
        this->proposal= proposal;
    }
    bool Vote(int nChoice);
    uint32_t GetVoteFromVersion(uint32_t nVersion);
    uint32_t GetFormattedVote() { return nFormattedVote; };
    std::string PrintBinary(uint32_t n);
};

#endif //HYPERSTAKE_VOTEOBJECT_H
