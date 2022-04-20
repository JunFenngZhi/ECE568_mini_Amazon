#ifndef _AURESPONSEHANDLER_H
#define _AURESPONSEHANDLER_H

#include "Server.h"
#include "protobufCommunication.hpp"
#include <vector>

class AUResponseHandler
{
private:
    vector<AOrderATruck> aorderatrucks;
    vector<AStartDeliver> astartdelivers;
    vector<int> seqNums;
    
public:
    AUResponseHandler(const AUResponse & r);
    ~AUResponseHandler();
    void handle();
};

#endif