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
  select a warehouse, which is closest to the order address. return the selected warehouse index.
*/
int selectWareHouse(const vector<Warehouse>& whList, const Order & order) {
  int index = -1;
  int minDistance = INT_MAX;
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
  check inventory for the given order. If enough, use different threads to 
  begin packing and ordering a truck. Otherwise, send APurchasemore command to 
  world.
*/
void processOrder(const Order & order) {
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  // determine to use which warehouse.
  Server::Ptr server = Server::get_instance();
  vector<Warehouse> whList = server->getWhList();
  int whIndex = selectWareHouse(whList, order);

  while (1) {
    try {
      // check inventory
      int itemId = order.getItemId();
      int itemAmt = order.getAmount();
      int packageId = order.getPackId();
      int upsid = order.getUPSId();
      int version = -1;
      bool isEnough = checkInventory(C.get(), itemId, itemAmt, whIndex, version);
      if (isEnough == true) {
        decreaseInventory(C.get(), whIndex, itemAmt, itemId, version);
        Server::disConnectDB(C.get());
        
        //create new thread to send APack Command to the world and AOrderATruck to the UPS
        thread t1(packOrder, order);
        t1.detach();
        thread t2(callATruck, order);
        t2.detach();
        return;
      }
      else {
        // save order
        server->order_lck.lock();
        server->orderQueue.push(order);
        server->order_lck.unlock();

        // create APurchasemore command
        ACommands ac;
        APurchaseMore * ap = ac.add_buy();
        ap->set_whnum(whIndex);
        AProduct * aproduct = ap->add_things();
        aproduct->set_id(itemId);
        aproduct->set_count(10 * itemAmt);
        aproduct->set_description(getDescription(C.get(), itemId));

        // add seqNum to this command.
        server->seqNum_lck.lock();
        int seqNum = server->curSeqNum;
        ap->set_seqnum(seqNum);
        server->curSeqNum++;
        server->seqNum_lck.unlock();

        // keep sending until get acked.
        pushWorldQueue(ac, seqNum);
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
  Send APack command to the world to pack given order.
*/
void packOrder(Order order) {
  Server::Ptr server = Server::get_instance();
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  vector<Warehouse>whList = server->getWhList();
  int whIndex = selectWareHouse(whList, order);
  int whouse_id = whList[whIndex].getID();
  
  // create Apack command
  ACommands acommand;
  APack * apack = acommand.add_topack();
  apack->set_whnum(whouse_id);
  apack->set_shipid(order.getPackId());
  AProduct * aproduct = apack->add_things();
  aproduct->set_id(order.getItemId());
  aproduct->set_count(order.getAmount());
  aproduct->set_description(getDescription(C.get(), order.getItemId()));

   // add seqNum to this command.
  server->seqNum_lck.lock();
  int seqNum = server->curSeqNum;
  apack->set_seqnum(seqNum);
  server->curSeqNum++;
  server->seqNum_lck.unlock();

  Server::disConnectDB(C.get());
  pushWorldQueue(acommand, seqNum);

}

/*
  Send AOrderATruck command to UPS to assign a truck to delivery the given package.
*/
void callATruck(Order order) {
  Server::Ptr server = Server::get_instance();

  vector<Warehouse>whList = server->getWhList();
  int whIndex = selectWareHouse(whList, order);
  

  //create AOrderATruck Command
  AUCommand aucommand;
  AOrderATruck * aOrderTruck = aucommand.add_order();
  int whouse_x = whList[whIndex].getX();
  int whouse_y = whList[whIndex].getY();
  int whouse_id = whList[whIndex].getID();
  int dest_x = order.getAddressX();
  int dest_y = order.getAddressY();
  aOrderTruck->set_packageid(order.getPackId());
  aOrderTruck->set_warehouselocationx(whouse_x);
  aOrderTruck->set_warehouselocationy(whouse_y);
  aOrderTruck->set_warehouseid(whouse_id);
  aOrderTruck->set_destinationx(dest_x);
  aOrderTruck->set_destinationy(dest_y);
  if (order.getUPSId() != -1) {
    aOrderTruck->set_upsid(order.getUPSId());
  }

  // add seqNum to this command.
  server->seqNum_lck.lock();
  int seqNum = server->curSeqNum;
  aOrderTruck->set_seqnum(seqNum);
  server->curSeqNum++;
  server->seqNum_lck.unlock();

  pushUpsQueue(aucommand, seqNum);
}

/* ------------------------ "Message Queue push functions" ------------------------ */
/*
  push AUCommand object into queue, and then send it to the UPS.
  It will keep pushing unitl server receives ack.
*/
void pushUpsQueue(const AUCommand & aucommand, int seqNum) {
  Server::Ptr server = Server::get_instance();
  while (1) {
    server->upsQueue.push(aucommand);
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (server->seqNumTable[seqNum] == true)
      break;
  }
}

/*
  push ACommand object into queue, and then send it to the world.
  It will keep pushing unitl server receives ack.
*/
void pushWorldQueue(const ACommands & acommand, int seqNum) {
  Server::Ptr server = Server::get_instance();
  while (1) {
    server->worldQueue.push(acommand);
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (server->seqNumTable[seqNum] == true)
      break;
  }
}

/* ------------------------ "Handle Response from World" ------------------------ */
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
  Server::Ptr server = Server::get_instance();
  server->order_lck.lock();
  Order order = server->orderQueue.front();
  server->orderQueue.pop();
  server->order_lck.unlock();

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
  updatePacked(C.get(), packageId);

  Server::disConnectDB(C.get());
}

/*
    create new thread, let it to process Loaded
    (Send AStartDeliver to UPS, ????????change order status as "loaded", when to set order
    status as delivering?)
*/
void processLoaded(ALoaded r) {
  //Connect the database
    unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  // get package id
  int packageId = r.shipid();

  //update order status as "loaded"
  updateloaded(C.get(),packageId);

  Server::disConnectDB(C.get());

  // Create AStartDeliver Command
  AUCommand aucommand;
  AStartDeliver * aStartDeliver = aucommand.add_deliver();
  aStartDeliver->set_packageid(packageId);

   // add seqNum to this command.
  Server::Ptr server = Server::get_instance();
  server->seqNum_lck.lock();
  int seqNum = server->curSeqNum;
  aStartDeliver->set_seqnum(seqNum);
  server->curSeqNum++;
  server->seqNum_lck.unlock();
  
  pushUpsQueue(aucommand, seqNum);
}



/* ------------------------ "Handle Response from UPS" ------------------------ */
/*
  receive UTruckArrive Response, keep checking database if the order status is packed 
  if order status is packed, then create APutOnTruck Command and send to World
*/
void processTruckArrived(UTruckArrive r) {
    //Connect the database
    unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

    //get package id and truck id and warehouse id
    int packageId = r.packageid();
    int truckId = r.truckid();
    int whId = -1;

    //check database if the order status is packed
    while(1) {
      if(checkOrderPacked(C.get(), packageId, whId)){
        break;
      }
    }
    //if the order is packed, then we need to send APutOnTruck to the world
    // and update order status as "loading"
    updateLoading(C.get(), packageId);

    //create APutOnTruck Command
    ACommands acommand;
    APutOnTruck * aPutOnTruck = acommand.add_load();
    aPutOnTruck->set_whnum(whId);
    aPutOnTruck->set_shipid(packageId);

     // add seqNum to this command.
    Server::Ptr server = Server::get_instance();
    server->seqNum_lck.lock();
    int seqNum = server->curSeqNum;
    aPutOnTruck->set_seqnum(seqNum);
    server->curSeqNum++;
    server->seqNum_lck.unlock();

    Server::disConnectDB(C.get());
    pushWorldQueue(acommand, seqNum);    
}

/*
  receive UDelivered Response, change order status to be 'delivered'
*/
void processUDelivered(UDelivered r) {
  //Connect the database
  unique_ptr<connection> C(Server::connectDB("mini_amazon", "postgres", "passw0rd"));

  //get shipid
  int packageId = r.packageid();

  //process this order status to be 'delivered'
  updateDelivered(C.get(), packageId);

  Server::disConnectDB(C.get());

}