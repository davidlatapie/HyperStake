// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "voteobject.h"
#include <string>

using namespace std;

//Convert char* into string showing its binary
string CVoteObject::PrintBinary(uint32_t n)
{
    string result;
    for (int i = 0; i < 32; i ++) {
        result.push_back( '0' + (n & 1) );
        n >>= 1;
    }

    reverse( result.begin(), result.end() );

    return result;
}

bool CVoteObject::Vote(int nVotersChoice)
{
//    int versionInFirstFour = CBlock::CURRENT_VERSION << 28;
//    cout << "-------------------------------------------------------------" << endl;
//    // cout << "CURRENT VERSION:            " << PrintBinary(versionInFirstFour) << endl;
//    cout << "voter's choice: " << nVotersChoice << endl;
//    // transform Choice to uint32_t
//    nChoice = static_cast<uint32_t>(nVotersChoice);
//    // move Choice to its place in Version
//    nChoice <<= proposal.GetShift();
//    cout << "choice after getting shift  " << PrintBinary(nChoice) << endl;
//    // combine Choice with Version
//    nFormattedVote = (versionInFirstFour |= nChoice);
//    // flag Voted
//    fVoted = true;
//    cout << "new nVersion:               " << PrintBinary(nFormattedVote) << endl;
//    return true;


    uint32_t versionInFirstFour = CBlock::CURRENT_VERSION << 28;
    int32_t binaryChoice = 0;

    if (nVotersChoice == 0) { // Abstain 00
        binaryChoice =  0;
    }
    else if (nVotersChoice == 3) { // Revise the proposal 01
        binaryChoice =  1;
    }
    else if (nVotersChoice == 2) { // No 10
        binaryChoice =  2;
    }
    else if (nVotersChoice == 1) { // Yes 11
        binaryChoice = 3;
    }

    nChoice = binaryChoice;
    nChoice <<= proposal.GetShift();
    nFormattedVote = (versionInFirstFour |= nChoice);
    return true;
}

uint32_t CVoteObject::GetVoteFromVersion(uint32_t nVersion)
{
    int diff = proposal.GetShift() - proposal.GetCardinals();
    uint32_t mask;
    mask = ((1 << (diff + 1)) - 1) << diff;
    return (nVersion & mask) >> diff;
}

