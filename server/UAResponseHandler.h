#ifndef _UARESPONSEHANDLER_H
#define _UARESPONSEHANDLER_H

#include "Server.h"
#include "protobufCommunication.hpp"
#include <vector>

class UAResponseHandler
{
private:
    vector<AOrderATruck> aorderatrucks;
    vector<AStartDeliver> astartdelivers;
    vector<int> seqNums;
    
public:
    UAResponseHandler(const UAResponse & r);
    ~UAResponseHandler();
    void handle();
};

#endif