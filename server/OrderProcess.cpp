#include "OrderProcess.h"


//APurchaseMore, create new thread, lend it to processPurchaseMore
//(Go to the database to increase the corresponding product inventory in the corresponding warehouse)
void processPurchaseMore(APurchaseMore r) {
    // first need to check if there are seqNum in executetable
    auto it = Server::executeTable.find(r.seqnum());
    // if not exists, insert seqNum in the set, else exit
    if(it == Server::executeTable.end()){
        Server::executeTable.insert(r.seqnum());
    } else {
        return;
    }

    // Connect the database
    connection * C = Server::connectDB("mini_amazon", "postgres", "passw0rd");

    // get warehouse id
    int whID = r.whnum();

    // get all products
    vector<AProduct> products;
    for(int i = 0; i < r.things_size(); i++){
        products.push_back(std::move(r.things(i)));
    }

    // process each product purchasemore
    for(int i = 0; i < products.size(); i++){
        int count = products[i].count();
        int productId = products[i].id();
        addInventory(C, whID, count, productId);
    }
    
}

//APacked, create new thread, lend it to processPacked
//(Go to the database and change the order status to 'packed')
void processPacked(APacked r) {
    //first need to check if there are seqNum in executetable
    auto it = Server::executeTable.find(r.seqnum());
    //if not exists, insert seqNum in the set, else exit
    if(it == Server::executeTable.end()){
        Server::executeTable.insert(r.seqnum());
    } else {
        return;
    }
    
    //Connect the database
    connection * C = Server::connectDB("mini_amazon", "postgres", "passw0rd");

    //get shipid
    int packageId = r.shipid();

    //process this order status to be 'packed'
    


}
//ALoaded, create new thread, lend it to processLoaded
//(Send AStartDeliver to UPS)
void processLoaded(ALoaded r) {

}
