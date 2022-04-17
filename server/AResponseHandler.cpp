 #include "AResponseHandler.h" 


 AResponseHandler::AResponseHandler(const AResponses& r, int worldfd) {
     //解析响应中的 APurchaseMore arrived
     for(int i =0; i < r.arrived_size(); i++) {
         //向成员变量apurchasems中添加APurchaseMore object
         apurchasems.push_back(r.arrived(i));
     }

     //解析响应中的 APacked ready
     for(int i =0; i < r.ready_size(); i++) {
         //向成员变量apurchasems中添加APurchaseMore object
         apackeds.push_back(r.ready(i));
     }

     //解析响应中的 ALoaded loaded
    for(int i =0; i < r.loaded_size(); i++) {
         //向成员变量apurchasems中添加APurchaseMore object
         aloadeds.push_back(r.loaded(i));
     }

     //初始化成员变量worldfd
     world_fd = worldfd;
 }


 void AResponseHandler::sendACKs(){
     unique_ptr<socket_out> world_out(new socket_out(world_fd));
     //1. 根据APurchaseMore中的#seq 向warehouse world #ACK
     for(int i = 0; i< apurchasems.size(); i++){
        if (sendMesgTo<int64_t>(apurchasems[i].seqnum()+1, world_out.get()) == false) {
        throw MyException("fail to send ack of APurchaseMore to world."); }
     } 
    //2. 根据APacked中的#seq 向warehouse world #ACK
    for(int i = 0; i< apackeds.size(); i++){
        if (sendMesgTo<int64_t>(apackeds[i].seqnum()+1, world_out.get()) == false) {
        throw MyException("fail to send ack of APacked to world."); }
     } 

    //3. 根据ALoaded中的#seq 向warehouse world发送中 #ACK
     for(int i = 0; i< aloadeds.size(); i++){
        if (sendMesgTo<int64_t>(aloadeds[i].seqnum()+1, world_out.get()) == false) {
        throw MyException("fail to send ack of ALoaded to world."); }
     } 
 }