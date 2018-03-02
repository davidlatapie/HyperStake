// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VOTEPROPOSAL_H
#define VOTEPROPOSAL_H

#include <iostream>
#include "main.h"
#include "serialize.h"
#include "votetally.h"

#define MAX_CHAR_NAME 10
#define MAX_CHAR_ABSTRACT 30
#define MAX_BLOCKS_IN_FUTURE 28800
#define MAX_CHECKSPAN 28800

class CVoteProposal
{
private:
    // what to call the proposal
    std::string strName;

    // what bit position in the block version
    uint8_t nShift;

    // where in the blockchain we start counting votes, nStartHeight
    unsigned int nStartHeight;

    // how far in the blockchain are we scanning
    int nCheckSpan;

    // number of bits in the block version used for one vote, nBitCount
    uint8_t nBitCount;

    // description of the proposal; may link to additional transactions
    std::string strDescription;
public:
    // the amount of HYP burnt when a proposal is made
    static const int64 FEE = 5 * COIN;

    void SetNull()
    {
        strName = "";
        nShift = 0;
        nStartHeight = 0;
        nCheckSpan = 0;
        nBitCount = 0;
        strDescription = "";
    }

    bool IsNull () const { return strName.empty(); }

    CVoteProposal()
    {
        SetNull();
    }

    CVoteProposal(std::string strName, uint8_t nShift, unsigned int nStartHeight, int nCheckSpan, uint8_t nBitCount,
                  std::string strDescription)
    {
        this->strName = strName;
        this->nShift = nShift;
        this->nStartHeight = nStartHeight;
        this->nCheckSpan = nCheckSpan;
        this->nBitCount = nBitCount;
        this->strDescription = strDescription;
    }

    IMPLEMENT_SERIALIZE
    (
       READWRITE(strName);
       READWRITE(nShift);
       READWRITE(nStartHeight);
       READWRITE(nCheckSpan);
       READWRITE(nBitCount);
       READWRITE(strDescription);
    )

    bool ConstructTransaction (CTransaction& tx) const;
    int GetShift() const { return nShift; }
    uint8_t GetBitCount() const { return nBitCount; }
    int GetCheckSpan() const { return nCheckSpan; }
    std::string GetName() const { return strName; }
    std::string GetDescription() const { return strDescription; }
    unsigned int GetStartHeight() const { return nStartHeight; }
    VoteLocation GetLocation() const;
    uint256 GetHash() const;

};

bool ProposalFromTransaction(const CTransaction& tx, CVoteProposal& proposal);

#endif //HYPERSTAKE_VOTEPROPOSAL_H
