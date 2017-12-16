// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "voteobject.h"
#include <string>

using namespace std;

//Convert char* into string showing its binary
string PrintBinary(uint32_t n)
{
    string result;
    for (int i = 0; i < 32; i ++) {
        result.push_back( '0' + (n & 1) );
        n >>= 1;
    }

    reverse( result.begin(), result.end() );

    return result;
}

bool CVoteObject::AddVoteToHeader(uint32_t& nVersion)
{

    int temp = CBlock::CURRENT_VERSION << 28;
    cout << "CURRENT VERSION:            " << PrintBinary(temp) << endl;
    nChoice = static_cast<uint32_t>(nChoice);
    nChoice <<= voteProposal.GetShift();
    cout << "nChoice after getting shift " << PrintBinary(nChoice) << endl;
    nFormattedVote = (temp |= nChoice);

    fVoted = true;
    cout << "new nVersion:               " << PrintBinary(nFormattedVote) << endl;
    return true;
}

