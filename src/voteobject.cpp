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

bool CVoteObject::AddVoteToVersion(uint32_t& nVersion)
{
    int versionInFirstFour = CBlock::CURRENT_VERSION << 28;
    cout << "CURRENT VERSION:            " << PrintBinary(versionInFirstFour) << endl;
    // transform Choice to uint32_t
    nChoice = static_cast<uint32_t>(nChoice);
    // move Choice to its place in Version 
    nChoice <<= proposal.GetShift();
    cout << "nChoice after getting shift " << PrintBinary(nChoice) << endl;
    // combine Choice with Version
    nFormattedVote = (versionInFirstFour |= nChoice);
    // flag Voted
    fVoted = true;
    cout << "new nVersion:               " << PrintBinary(nFormattedVote) << endl;
    return true;
}

uint32_t CVoteObject::GetVoteFromVersion(uint32_t nVersion)
{
    // Create a mask that will 0 out all bits that are not in the bits for this proposal 
    uint32_t nVoteMask = 1;
    for ( int i = 0; i < proposal.GetCardinals(); i++) {
        nVoteMask |= 1;
        nVoteMask <<= 1;
    }
    nVoteMask <<= (proposal.GetShift() - proposal.GetCardinals());
    //cout << "X: " << PrintBinary(nVoteMask) << :endl;

    return nVersion & nVoteMask;
}

