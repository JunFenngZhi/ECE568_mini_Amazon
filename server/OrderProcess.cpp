#include "OrderProcess.h"


//APurchaseMore, create new thread, lend it to processPurchaseMore
//(Go to the database to increase the corresponding product inventory in the corresponding warehouse)
void processPurchaseMore(APurchaseMore r) {
    



    //get warehouse id
    int whID = r.whnum();

    // get all products
    vector<AProduct> products;
    for(int i = 0; i < r.things_size(); i++){
        products.push_back(std::move(r.things(i)));
    }

    // process each product purchasemore
    
    


}

//APacked, create new thread, lend it to processPacked
//(Go to the database and change the order status to 'packed')
void processPacked(APacked r) {

}
//ALoaded, create new thread, lend it to processLoaded
//(Send AStartDeliver to UPS)
void processLoaded(ALoaded r) {

}