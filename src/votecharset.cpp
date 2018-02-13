#include <map>
#include <iostream>

#include "votecharset.h"
#include "uint256.h"

std::map<std::string, uint160 > charcodes = 
    {
        {"e", 0x25},
        {"s", 0x33},
        {"t", 0x34},
    };

bool transformStringChars(std::string strFromUser, std::string &returnString)
{
    std::string strIncoming = strFromUser.c_str();
    std::transform(strIncoming.begin(), strIncoming.end(), strIncoming.begin(), ::tolower);
    uint160 v;
    v <<= 6;

    std::string c;
    for (int i = 0; i < strIncoming.size(); i++) {
        c = strIncoming[i];
        if (charcodes.count(c)) {
            v |= uint160( charcodes.find(c) -> second );
        }
        else { return false; }
    }
    std::cout << "testing text: " << v.GetHex() << std::endl;
    returnString = v.GetHex();
    return true;
}
