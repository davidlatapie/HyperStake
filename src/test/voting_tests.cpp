#include <map>
#include <string>
#include <boost/test/unit_test.hpp>
#include "json/json_spirit_writer_template.h"

#include "main.h"
#include "wallet.h"
#include "../voteproposal.h"
#include "../voteobject.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(voting_tests)


BOOST_AUTO_TEST_CASE(proposal_serialization)
{
    std::cout << "testing proposal serialization\n";

    // name of issue
    std::string strName = "proposal1";
    // check version for existing proposals Shift
    uint8_t nShift = 20;
    // start time - will be changed to int StartHeight. unix time stamp
    int64 nStartTime =  10000000;
    // number of blocks with votes to count
    int nCheckSpan = 1000;
    // cardinal items to vote on - convert to uint8 CheckSpan
    uint8_t nCardinals = 2;
    // description of issue - will go in different tx
    std::string strDescription = "test_description";

    CVoteProposal proposal(strName, nShift, nStartTime, nCheckSpan, nCardinals, strDescription);

    //! Add the constructed proposal to a partial transaction
    CTransaction tx;
    BOOST_CHECK_MESSAGE(proposal.ConstructTransaction(tx), "failed to construct tx");

    BOOST_CHECK_MESSAGE(tx.IsProposal(), "Transaction is not a proposal!");

    CVoteProposal proposal2;
    BOOST_CHECK_MESSAGE(ProposalFromTransaction(tx, proposal2), "Failed to deserialize");

    //! Create a tx that can be used as an input in the actual proposal tx
    CTransaction txFunding;
    CTxOut outTemp(10 * COIN, CScript());
    txFunding.vout.push_back(outTemp);
    mempool.addUnchecked(txFunding.GetHash(), txFunding);

    //! Add an input to the proposal tx
    tx.vin.push_back(CTxIn(txFunding.GetHash(), 0));
    tx.vout[0].nValue = MIN_TXOUT_AMOUNT;
    cout << tx.ToString() << "\n";

    //! Return anything beyond the proposal fee (5 HYP) as change UTXO
    int64 nFee = 5 * COIN;
    nFee -= MIN_TXOUT_AMOUNT; //Had to assign mintxamt to the utxo containing the proposal
    int64 nChange = outTemp.nValue - nFee;
    if (nChange > 500) {
        CKey key;
        key.MakeNewKey(true);

        CScript scriptReturn;
        scriptReturn << OP_DUP << OP_HASH160 << ToByteVector(Hash160(scriptReturn)) << OP_EQUALVERIFY << OP_CHECKSIG;
        CTxOut out(nChange, scriptReturn);

        //Add the change output to the new transaction
        tx.vout.push_back(out);
    }

    BOOST_CHECK_MESSAGE(tx.CheckTransaction(), "check transaction failed");

    CTxDB txdb("r");
    BOOST_CHECK_MESSAGE(tx.AcceptToMemoryPool(txdb, false), "transaction not accepted to mempool");

}

BOOST_AUTO_TEST_SUITE_END()