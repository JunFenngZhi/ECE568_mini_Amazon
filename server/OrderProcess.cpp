#include "OrderProcess.h"

/*
  parse order from string to Order object. save it to the database.
*/
void parseOrder(string msg) {
  cout << "successfully receive order request.\n";
  Order order(msg);
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  saveOrderInDB(C.get(), order);
  Server::disConnectDB(C.get());

  processOrder(order);

 
}

/*
  check inventory for the given order. If enough, use different threads to 
  begin packing and ordering a truck. Otherwise, send APurchasemore command to 
  world.
*/
void processOrder(const Order & order) {
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  // determine to use which warehouse.
  int whIndex = selectWareHouse(order);

  while (1) {
    try {
      // check inventory
      int itemId = order.getItemId();
      int itemAmt = order.getAmount();
      int version = -1;
      bool isEnough = checkInventory(C.get(), itemId, itemAmt, whIndex, version);
      if (isEnough == true) {
        decreaseInventory(C.get(), whIndex, -1 * itemAmt, itemId, version);
        Server::disConnectDB(C.get());
        //create new thread to begin packing and order a truck
        return;
      }
      else {
        // save order
        Server::order_lck.lock();
        Server::orderQueue.push(order);
        Server::order_lck.unlock();

        // create APurchasemore command
        ACommands ac;
        APurchaseMore * ap = ac.add_buy();
        ap->set_whnum(whIndex);
        AProduct * aproduct = ap->add_things();
        aproduct->set_id(itemId);
        aproduct->set_count(10 * itemAmt);
        aproduct->set_description(getDescription(C.get(), itemId));

        // add seqNum to this command.
        Server::seqNum_lck.lock();
        int seqNum = Server::curSeqNum;
        ap->set_seqnum(seqNum);
        Server::curSeqNum++;
        Server::seqNum_lck.unlock();
        
        // keep sending until get acked.
        while (1) {  
          Server::worldQueue.push(ac);
          this_thread::sleep_for(std::chrono::milliseconds(1000));
          if (Server::seqNumTable[seqNum] == true)
            break;
        }
        break;
      }
    }
    catch (const VersionErrorException & e) {
      std::cerr << e.what() << '\n';
    }
  }

  Server::disConnectDB(C.get());
}

/*
  select a warehouse, which is closest to the order address. return the selected warehouse index.
*/
int selectWareHouse(const Order & order) {
  int index = -1;
  int minDistance = INT_MAX;
  vector<Warehouse> whList = Server::getWhList();
  for (int i = 0; i < whList.size(); i++) {
    int delta_x = abs(whList[i].getX() - order.getAddressX());
    int delta_y = abs(whList[i].getY() - order.getAddressY());
    if ((delta_x * delta_x + delta_y * delta_y) < minDistance) {
      minDistance = delta_x * delta_x + delta_y * delta_y;
      index = i;
    }
  }
  return index;
}

/*
    create new thread, let it to process PurchaseMore
    (Go to the database to increase the corresponding product inventory in the corresponding warehouse)
*/
void processPurchaseMore(APurchaseMore r) {
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  // get warehouse id
  int whID = r.whnum();

  // get all products
  vector<AProduct> products;
  for (int i = 0; i < r.things_size(); i++) {
    products.push_back(std::move(r.things(i)));
  }

  // process each product purchasemore
  for (int i = 0; i < products.size(); i++) {
    int count = products[i].count();
    int productId = products[i].id();
    addInventory(C.get(), whID, count, productId);
  }
  Server::disConnectDB(C.get());

  // process previous saved order
  Server::order_lck.lock();
  Order order = Server::orderQueue.front();
  Server::orderQueue.pop();
  Server::order_lck.unlock();

  processOrder(order);
  
}

/*
    create new thread, let it to process Packed
    (Go to the database and change the order status to 'packed')
*/
void processPacked(APacked r) {
  //Connect the database
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  //get shipid
  int packageId = r.shipid();

  //process this order status to be 'packed'
}

/*
    create new thread, let it to process Loaded
    (Send AStartDeliver to UPS)
*/
void processLoaded(ALoaded r) {
}

