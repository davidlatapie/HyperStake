// Copyright (c) 2012-2013 The PPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef PPCOIN_KERNEL_H
#define PPCOIN_KERNEL_H

#include "main.h"

// MODIFIER_INTERVAL: time to elapse before new modifier is computed
static const unsigned int MODIFIER_INTERVAL = 6* 60* 60; 
static const unsigned int MODIFIER_INTERVAL_TESTNET = 60;
extern unsigned int nModifierInterval;
extern unsigned int getIntervalVersion(bool fTestNet);

// MODIFIER_INTERVAL_RATIO:
// ratio of group interval length between the last group and the first group
static const int MODIFIER_INTERVAL_RATIO = 3;

// Compute the hash modifier for proof-of-stake
bool ComputeNextStakeModifier(const CBlockIndex* pindexPrev, uint64& nStakeModifier, bool& fGeneratedStakeModifier);

// Check whether stake kernel meets hash target
// Sets hashProofOfStake on success return
bool CheckStakeKernelHash(unsigned int nBits, const CBlock& blockFrom, unsigned int nTxPrevOffset, const CTransaction& txPrev, 
	const COutPoint& prevout, unsigned int& nTimeTx, unsigned int nInterval, bool fCheck, uint256& hashProofOfStake, CBigNum& bnCoinWeight, bool fPrintProofOfStake=false);
uint256 stakeHash(unsigned int nTimeTx, unsigned int nTxPrevTime, CDataStream ss, unsigned int prevoutIndex, unsigned int nTxPrevOffset, unsigned int nTimeBlockFrom);
bool stakeTargetHit(uint256 hashProofOfStake, unsigned int nAge, int64 nValueIn, CBigNum bnTargetPerCoinDay, CBigNum& bnCoinWeight);

// Check kernel hash target and coinstake signature
// Sets hashProofOfStake on success return
bool CheckProofOfStake(const CTransaction& tx, unsigned int nBits, uint256& hashProofOfStake);

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(int64 nTimeBlock, int64 nTimeTx);

// Get stake modifier checksum
unsigned int GetStakeModifierChecksum(const CBlockIndex* pindex);

// Check stake modifier hard checkpoints
bool CheckStakeModifierCheckpoints(int nHeight, unsigned int nStakeModifierChecksum);

// Get time weight using supplied timestamps
int64 GetWeight(int64 nIntervalBeginning, int64 nIntervalEnd);
int64 GetWeight2(int64 nIntervalBeginning, int64 nIntervalEnd);

#endif // PPCOIN_KERNEL_H
