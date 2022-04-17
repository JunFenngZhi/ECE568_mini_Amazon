 #include "AResponseHandler.h" 
 #include "OrderProcess.h"



 AResponseHandler::AResponseHandler(const AResponses& r) {
     for(int i =0; i < r.arrived_size(); i++) {
        apurchasemores.push_back(std::move(r.arrived(i)));
        seqNums.push_back(std::move(r.arrived(i).seqnum()));
     }

     for(int i =0; i < r.ready_size(); i++) {
         apackeds.push_back(std::move(r.ready(i)));
         seqNums.push_back(std::move(r.ready(i).seqnum()));
     }

     for(int i =0; i < r.loaded_size(); i++) {
         aloadeds.push_back(std::move(r.loaded(i)));
         seqNums.push_back(std::move(r.loaded(i).seqnum()));
     }
 }

/*
    use a new thread to handle different type of responses, and ack those messages.
*/
 void AResponseHandler::handle(){
    // ACK seqNums(create ACommands, and put it into queue)
    ACommands acommand;
    for(int i =0; i < seqNums.size(); i++) {
        acommand.set_acks(i,seqNums[i]);
    }
    // push it to the queue
    Server::worldQueue.push(acommand);

    // create a new thread to handle different of responses.
    for(auto r:apurchasemores){
        thread t(processPurchaseMore, r);  
        t.detach();
    }

    for(auto r:apackeds){
        thread t(processPacked, r);  
        t.detach();
    }

    for(auto r:aloadeds){
        thread t(processLoaded, r);  
        t.detach();
    }

 }


