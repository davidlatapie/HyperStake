// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "voteproposalmanager.h"
#include "voteproposal.h"

std::string CVoteProposalManager::GetProposals()
{
    std::string strReturn;
    for (std::map<uint256, CTransaction>::iterator it = mapProposals.begin(); it != mapProposals.end(); it++)
    {
        strReturn += (*it).first.GetHex() + "\n";
    }

    return strReturn;
}

