// Copyright (c) 2017 The HyperStake Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef HYPERSTAKE_VOTEPROPOSALMANAGER_H
#define HYPERSTAKE_VOTEPROPOSALMANAGER_H

class CVoteProposalManager
{
private:
    int nPendingCount;
public:
    CVoteProposalManager()
    {
    }
    std::string GetProposals();
    int GetPendingCount() {return nPendingCount;};
};

#endif //HYPERSTAKE_VOTEPROPOSALMANAGER_H
