// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "voteobject.h"
#include "main.h"

using namespace std;

bool CVoteObject::Vote(int nVotersChoice)
{
    uint32_t versionInFirstFour = CBlock::CURRENT_VERSION << 28;
    uint32_t binaryChoice = 0;

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
    nChoice <<= nShift;
    nFormattedVote = (versionInFirstFour |= nChoice);
    return true;
}

uint32_t CVoteObject::GetVoteFromVersion(uint32_t nVersion)
{
    int diff = nShift - nCardinals;
    uint32_t mask;
    mask = ((1 << (diff + 1)) - 1) << diff;
    return (nVersion & mask) >> diff;
}

