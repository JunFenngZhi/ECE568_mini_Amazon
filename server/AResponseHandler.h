#include "Server.h"
#include "protobufCommunication.hpp"

class AResponseHandler
{
private:
    vector<APurchaseMore> apurchasems;
    vector<APacked> apackeds;
    vector<ALoaded> aloadeds;
    int world_fd;
public:
    AResponseHandler(const AResponses& r, int worldfd);
    ~AResponseHandler(){}
    // 根据Response的种类，向World发送对应的object(APurchaseMore, APacked, ALoaded)中的#seq
    void sendACKs();
    //APurchaseMore,创建新的线程, lend it to processPurchaseMore(去数据库中将对应warehouse中的对应product库存增加)
    void processPurchaseMore();
    //APacked, 创建新的线程, lend it to processPacked(去数据库修改order status变为 ’packed‘)
    void processPacked();
    //ALoaded, 创建新的线程, lend it to processLoaded(向UPS发送 AStartDeliver)
    void processLoaded();
    

};

