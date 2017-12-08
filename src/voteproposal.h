//
// Created by carrie on 12/8/17.
//

#ifndef HYPERSTAKE_VOTEPROPOSAL_H
#define HYPERSTAKE_VOTEPROPOSAL_H

#endif //HYPERSTAKE_VOTEPROPOSAL_H

class CVoteProposal
{
private:
    // what to call the proposal
    std::string strName;

    // what bit position in the block version
    uint_8 nShift;

    // number of bits in the block version used for one vote
    uint_8 nBitCount;

    // where in the blockchain we start counting votes
    int nStartHeight;

    // how far in the blockchain are we scanning
    int  nCheckSpan;

    // description of the proposal; may link to additional transactions
    CTransaction strDescription;
public:
    void SetNull()
    {
        strName = "";
        nShift = 0;
        nBitCount = 0;
        nCheckSpan = 0;
    }

    CVoteProposal()
    {
        SetNull();
    }

    CVoteProposal(  std::string strName,
                    uint_8 nShift,
                    uint_8 nBitCount,
                    int nStartHeight,
                    int nCheckSpan,
                    CTransaction strDescription)
    {
        this->strName = strName;
        this->nShift = nShift;
        this->nBitCount = nBitCount;
        this->nStartHeight = nStartHeight;
        this->nCheckSpan = nCheckSpan;
        this->strDescription = strDescription;
    }

    std::string GetName() { return strName; }
    void SetName(std::string strName) { this->strName = strName; }
    int GetShift() { return nShift; }
    void SetShift(int nShift) { this->nShift = nShift; }
    int GetBitCount() { return nBitCount; }
    void SetBitCount(int nBitCount) { this->nBitCount = nBitCount; }
    int GetCheckSpan() { return nBlockCheckSpan; }
    void SetCheckSpan(int nCheckSpan) { this->nBlockCheckSpan = nCheckSpan; }
    std::string ToString() { return }
}