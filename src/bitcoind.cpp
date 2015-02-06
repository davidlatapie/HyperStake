// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bitcoinrpc.h"
#include "init.h"
#include <boost/algorithm/string/predicate.hpp>

//////////////////////////////////////////////////////////////////////////////
//
// Start
//
bool AppInit(int argc, char* argv[])
{
    bool fRet = false;
    try
    {
        //
        // Parameters
        //
        // If Qt is used, parameters/bitcoin.conf are parsed in qt/bitcoin.cpp's main()
        ParseParameters(argc, argv);
        if (!boost::filesystem::is_directory(GetDataDir(false)))
        {
            fprintf(stderr, "Error: Specified directory does not exist\n");
            Shutdown(NULL);
        }
        ReadConfigFile(mapArgs, mapMultiArgs);
        
        if (mapArgs.count("-?") || mapArgs.count("--help"))
        {
            // First part of help message is specific to bitcoind / RPC client
            std::string strUsage = _("HyperStake version") + " " + FormatFullVersion() + "\n\n" +
            _("Usage:") + "\n" +
            "  HyperStaked [options]                     " + "\n" +
            "  HyperStaked [options] <command> [params]  " + _("Send command to -server or HyperStaked") + "\n" +
            "  HyperStaked [options] help                " + _("List commands") + "\n" +
            "  HyperStaked [options] help <command>      " + _("Get help for a command") + "\n";
            
            strUsage += "\n" + HelpMessage();
            
            fprintf(stdout, "%s", strUsage.c_str());
            return false;
        }
        
        // Command-line RPC
        for (int i = 1; i < argc; i++)
            if (!IsSwitchChar(argv[i][0]) && !boost::algorithm::istarts_with(argv[i], "HyperStake:"))
                fCommandLine = true;
        
        if (fCommandLine)
        {
            int ret = CommandLineRPC(argc, argv);
            exit(ret);
        }
        
        fRet = AppInit2();
    }
    catch (std::exception& e) {
        PrintException(&e, "AppInit()");
    } catch (...) {
        PrintException(NULL, "AppInit()");
    }
    if (!fRet)
        Shutdown(NULL);
    return fRet;
}

extern void noui_connect();
int main(int argc, char* argv[])
{
    bool fRet = false;
    
    // Connect bitcoind signal handlers
    noui_connect();
    
    fRet = AppInit(argc, argv);
    
    if (fRet && fDaemon)
        return 0;
    
    return 1;
}