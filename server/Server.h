#ifndef _SERVER_H
#define _SERVER_H

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "./protobuf/world_amazon.pb.h"
#include "./protobuf/AUprotocolV3.pb.h"
#include "Warehouse.h"
#include "exception.h"
#include "Order.h"
#include "AResponseHandler.h"
#include "ThreadSafe_queue.h"

using namespace std;
using namespace pqxx;

const int MAX_SEQNUM = 65536;

class Server {
 private:
  string webPortNum;
  string worldHostName;
  string worldPortNum;
  string upsHostName;
  string upsPortNum;
  int worldID;
  int n_warehouse;           // number of warehouse
  int wh_distance;           // distance between neighbour warehouse
  vector<Warehouse> whList;  // list of warehouse
  int ups_fd;
  int world_fd;
  static vector<bool> seqNumTable; // mark whether seqNum from amazon is acked.
  size_t curSeqNum;         // the sequence number to be used next(add in send thread)
  unordered_set<int> executeTable; // mark whether specific response has been executed
  ThreadSafe_queue<ACommands> worldQueue; // store the ACommands objects that need to be sent to the World.
  ThreadSafe_queue<AUCommand> upsQueue; // store the AUCommand objects that need to be sent to the UPS.
  // mutex IO_lck;
  // condition_variable IO_cv;

 private:
  connection * connectDB(string dbName, string userName, string W);
  void initializeDB(connection * C);
  void disConnectDB(connection * C);
  void acceptOrderRequest();
  void initializeWorld();
  void getWorldIDFromUPS();
  void handleOrderRequest(string requestMsg);
  int selectWareHouse(const Order& order);
  void keepReceivingMsg();
  void keepSendingMsg();

  private:
   friend class AResponseHandler;

 public:
  Server();
  ~Server() {}
  void run();
};

#endif
