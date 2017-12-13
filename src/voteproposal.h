//
// Created by carrie on 12/8/17.
//

#ifndef VOTEPROPOSAL_H
#define VOTEPROPOSAL_H

#include <iostream>

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
    void SetNull()
    {
        strName = "";
        nShift = 0;
        nStartTime = 0;
        nCheckSpan = 0;
        nCardinals = 0;
        strDescription = "";
    }

    CVoteProposal()
    {
        SetNull();
    }

    CVoteProposal(  std::string strName,
                    uint8_t nShift,
                    int nStartTime,
                    int nCheckSpan,
                    uint8_t nCardinals,
                    std::string strDescription)
    {
        this->strName = strName;
        this->nShift = nShift;
        this->nStartTime = nStartTime;
        this->nCheckSpan = nCheckSpan;
        this->nCardinals = nCardinals;
        this->strDescription = strDescription;
    }

};

#endif //HYPERSTAKE_VOTEPROPOSAL_H
