// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "voteproposal.h"

using namespace std;


/**
* The vote proposal is serialized and added to a CTransaction as a data object via OP_RETURN transaction output.
* The transaction is marked as a proposal by marking the first 4 bytes as "PROP" in ASCII
*/
bool CVoteProposal::ConstructTransaction(CTransaction& tx)
{
    if (tx.vin.empty()) {
        printf("%s : transaction does not have any inputs!\n", __func__);
        return false;
    }

    if (this->IsNull()) {
        printf("%s : vote proposal is null!\n", __func__);
        return false;
    }

    CScript scriptProposal;
    scriptProposal << OP_RETURN << 0x70726f70; //"PROP" in ASCII

    //serialize the vote proposal
    CDataStream serializedProposal(SER_NETWORK, PROTOCOL_VERSION);
    serializedProposal << *this;
    vector<unsigned char> vchData(serializedProposal.begin(), serializedProposal.end());

    //Add serialized proposal to the script
    scriptProposal << vchData.size() << vchData;

    //Create txout and add it to the transaction
    CTxOut out;
    out.scriptPubKey = scriptProposal;
    out.nValue = 0;
    tx.vout.push_back(out);

    if (tx.GetSerializeSize(SER_NETWORK, PROTOCOL_VERSION)) {
        printf("%s : transaction size is too large!\n", __func__);
        return false;
    }

    return true;
}
