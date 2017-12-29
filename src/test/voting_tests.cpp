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
    proposal.ConstructTransaction(tx);

    BOOST_CHECK_MESSAGE(tx.IsProposal(), "Transaction is not a proposal!");

    CVoteProposal proposal2;
    BOOST_CHECK_MESSAGE(ProposalFromTransaction(tx, proposal2), "Failed to deserialize");

}

BOOST_AUTO_TEST_SUITE_END()