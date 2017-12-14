// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

bool CVoteObject::AddVoteToHeader(uint32_t& nVersion)
{
    if (nFormattedVote & ~HEADERMASK) {
        std:
        cout << "first four bits are not clean" << std::endl;
        return false;
    }
    nVersion | FormatVote(nCardinals);
    return true;
}

bool CVoteObject::FormatVote()
{
// change int nCardinals to uint32_t
    *nFormattedVote = static_cast<uint32_t>(nCardinals);
    return true;
}
