// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef HYPERSTAKE_VOTEOBJECT_H
#define HYPERSTAKE_VOTEOBJECT_H

#include "voteproposal.h"

class CVoteObject
{
private:
    uint32_t nChoice;
    uint32_t nFormattedVote;
    bool fVoted;
    CVoteProposal proposal;
public:

    void SetNull()
    {
        nChoice = 0;
        nFormattedVote = 0;
        fVoted = false;
        proposal = CVoteProposal();
    }

    bool IsNull () { return proposal.IsNull(); }

    CVoteObject()
    {
        SetNull();
    }

    CVoteObject(CVoteProposal proposal)
    {
        this->proposal= proposal;
    }

    IMPLEMENT_SERIALIZE
    (
            READWRITE(nChoice);
            READWRITE(nFormattedVote);
            READWRITE(fVoted);
            READWRITE(proposal);
    )

    bool Vote(int nVotersChoice);
    uint32_t GetVoteFromVersion(uint32_t nVersion);
    uint32_t GetFormattedVote() { return nFormattedVote; };
    CVoteProposal GetProposal() { return proposal; };
    std::string PrintBinary(uint32_t n);
};

#endif //HYPERSTAKE_VOTEOBJECT_H
