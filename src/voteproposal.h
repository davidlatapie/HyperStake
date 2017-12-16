// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VOTEPROPOSAL_H
#define VOTEPROPOSAL_H

#include <iostream>
#include "main.h"
#include "serialize.h"

class CVoteProposal
{
private:
    // what to call the proposal
    std::string strName;

    // what bit position in the block version
    uint8_t nShift;

    // where in the blockchain we start counting votes, nStartHeight
    int nStartTime;

    // how far in the blockchain are we scanning
    int  nCheckSpan;

    // number of bits in the block version used for one vote, nBitCount
    uint8_t nCardinals;

    // description of the proposal; may link to additional transactions
    std::string strDescription;
public:
    // the amount of HYP burnt when a proposal is made
    static const int64 FEE = 5 * COIN;

    void SetNull()
    {
        strName = "";
        nShift = 0;
        nStartTime = 0;
        nCheckSpan = 0;
        nCardinals = 0;
        strDescription = "";
    }

    bool IsNull () { return strName == ""; }

    CVoteProposal()
    {
        SetNull();
    }

    CVoteProposal(std::string strName, uint8_t nShift, int nStartTime, int nCheckSpan, uint8_t nCardinals,
                  std::string strDescription)
    {
        this->strName = strName;
        this->nShift = nShift;
        this->nStartTime = nStartTime;
        this->nCheckSpan = nCheckSpan;
        this->nCardinals = nCardinals;
        this->strDescription = strDescription;
    }

    IMPLEMENT_SERIALIZE
    (
       READWRITE(strName);
       READWRITE(nShift);
       READWRITE(nStartTime);
       READWRITE(nCheckSpan);
       READWRITE(nCardinals);
       READWRITE(strDescription);
    )

    bool ConstructTransaction(CTransaction& tx);
    int GetShift() {return nShift;};

};

#endif //HYPERSTAKE_VOTEPROPOSAL_H
