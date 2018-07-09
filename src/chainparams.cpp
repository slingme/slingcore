// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Slingcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "genesis.h"
#include "chainparamsseeds.h"
#include "arith_uint256.h"
#include "libzerocoin/Params.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000006793e527cc2bfc0d9bb434873c18ab011c54a8ef2b9584f6d97a1dbe906"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1531098372, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x000001a31c72349a96269ea4c611d6b2f1e122896b1f846676abb5a805b39d4d"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1531090865,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x13;
        pchMessageStart[1] = 0x37;
        pchMessageStart[2] = 0x13;
        pchMessageStart[3] = 0x37;
        vAlertPubKey = ParseHex("04cb19e0c159760cff447973d1f9f24e15d68303f34948de0f5b2df4969237e3a9b9cc5b2105435262fa5044710ecbc91bda7b019fa231883117cc181f17d05c1c");
        nDefaultPort = 32137;
        bnProofOfWorkLimit = uint256S("000007fff0000000000000000000000000000000000000000000000000000000");

        nPoWAveragingWindow = 17;
        nPoWMaxAdjustDown = 32; // 32% adjustment down
        nPoWMaxAdjustUp = 16; // 16% adjustment up
        nPoWTargetSpacing = 2.5 * 60; // Sling: 2.5 minutes //TODO: CryptoDJ, increase to 7 minutes

        nSubsidyHalvingInterval = 200000000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 90; // Slingcoin: 1.5 minutes
        nTargetSpacingSlowLaunch = 10 * 60;  // Slingcoin: 10 minutes (Slow launch - Block 288)
        nTargetSpacing = 1 * 60;  // Slingcoin: 60 Seconds
        nMaturity = 37;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 2000000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 3500;  //TODO: (Sling) Increase Last PoW Block
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 1000;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 1525981707; // 14 - apr - 2018
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        fNewChain = false;

        //
        genesis = CreateGenesisBlock(1531098372, 272872, UintToArith256(bnProofOfWorkLimit).GetCompact(), 1, (.35 * COIN));
        if(fNewChain == true) { MineGenesis(genesis, bnProofOfWorkLimit, true); }

        hashGenesisBlock = genesis.GetHash();
        if (!fNewChain) {
            assert(hashGenesisBlock == uint256("0x000006793e527cc2bfc0d9bb434873c18ab011c54a8ef2b9584f6d97a1dbe906"));
            assert(genesis.hashMerkleRoot == uint256("0x3f18381a548302c0c3bf33bdf0e0efd61968024a217ae587bbe670c2f6dd632d"));
        }

        //TODO: (Sling) Add DNS seeders  
        vSeeds.push_back(CDNSSeedData("slingmarket.net", "seed.slingmarket.net"));     // Primary DNS Seeder from slingmarket.net

        base58Prefixes[PUBKEY_ADDRESS] = list_of(63).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS] = list_of(85).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]     = list_of(153).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 1135
        base58Prefixes[EXT_COIN_TYPE] = list_of(0x04)(0x88)(0xB2)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
		strSporkKey = "0467662d0bd65de02cd1a7325968c116a2ae9ad3c55ecdb6509c2a2c264f37981068f16c5fb3986e84347f0b14316c445d48a1d47fcd51161270b05aad29aa323b";  //TODO new key here
        strObfuscationPoolDummyAddress = "";
        nStartMasternodePayments = genesis.nTime + 7350;

        /** Zerocoin */
        zerocoinModulus = ""; //TODO: (Sling) add new modulus
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x14;
        pchMessageStart[1] = 0x36;
        pchMessageStart[2] = 0x14;
        pchMessageStart[3] = 0x36;
        vAlertPubKey = ParseHex("04cb19e0c159760cff447973d1f9f24e15d68303f34948de0f5b2df4969237e3a9b9cc5b2105435262fa5044710ecbc91bda7b019fa231883117cc181f17d05c1c");
        nDefaultPort = 32237;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Slingcoin: 1 day
        nTargetSpacing = 1 * 60;  // Slingcoin: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 5;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1524711188;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        fNewChain = false;

        //Testnet: Sunday, July 8, 2018 11:01:05 PM
        genesis = CreateGenesisBlock(1531090865, 2622957, UintToArith256(bnProofOfWorkLimit).GetCompact(), 1, (.35 * COIN));
        if(fNewChain == true) { MineGenesis(genesis, bnProofOfWorkLimit, true); }

        hashGenesisBlock = genesis.GetHash();
        if (!fNewChain) {
            assert(hashGenesisBlock == uint256("0x000001a31c72349a96269ea4c611d6b2f1e122896b1f846676abb5a805b39d4d"));
            assert(genesis.hashMerkleRoot == uint256("0x3f18381a548302c0c3bf33bdf0e0efd61968024a217ae587bbe670c2f6dd632d"));
        }
	    
        //TODO: (Sling) Add DNS seeders  
        vSeeds.push_back(CDNSSeedData("slingmarket.net", "seed.testnet.slingmarket.net"));     // Primary DNS Seeder from slingmarket.net
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet sling addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet sling script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet sling BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet sling BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet sling BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0467662d0bd65de02cd1a7325968c116a2ae9ad3c55ecdb6509c2a2c264f37981068f16c5fb3986e84347f0b14316c445d48a1d47fcd51161270b05aad29aa323b";
        strObfuscationPoolDummyAddress = "";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x15;
        pchMessageStart[1] = 0x35;
        pchMessageStart[2] = 0x15;
        pchMessageStart[3] = 0x35;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Slingcoin: 1 day
        nTargetSpacing = 1 * 60;        // Slingcoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1515524400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 732084;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 32337;
        //assert(hashGenesisBlock == uint256("0x0"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 32437;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
