#ifndef _SERVER_H
#define _SERVER_H

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <thread>
#include <vector>
#include <queue>

#include "./protobuf/AUprotocolV3.pb.h"
#include "./protobuf/world_amazon.pb.h"
#include "AResponseHandler.h"
#include "Order.h"
#include "ThreadSafe_queue.h"
#include "Warehouse.h"
#include "exception.h"

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
  static vector<Warehouse> whList;  // list of warehouse 
  int ups_fd;
  int world_fd;
  
 public:
  // maintain seqNum from amazon
  static vector<bool>
      seqNumTable;          // record whether commands with specific seqNum is acked.
  static size_t curSeqNum;  // next seqNum to be used.
  static mutex seqNum_lck;  // mutex used to lock seqNum

  // Records whether a response with a specific sequence number is executed
  // if seqNum is in executeTable, this response has been executed.
  static unordered_set<int> executeTable_World;
  static unordered_set<int> executeTable_Ups;

  //message queue, transfer message to sending threads
  static ThreadSafe_queue<ACommands> worldQueue;
  static ThreadSafe_queue<AUCommand> upsQueue;

  //order queue. save orders for later processing
  static queue<Order> orderQueue;
  static mutex order_lck;

 private:
  void acceptOrderRequest();
  void initializeWorld();
  void getWorldIDFromUPS();
  void keepReceivingMsg();
  void keepSendingMsgToWorld();
  void keepSendingMsgToUps();

 public:  //如果使用线程池，放回private
  static connection * connectDB(string dbName, string userName, string password);
  static void initializeDB(connection * C);
  static void disConnectDB(connection * C);
  static vector<Warehouse> getWhList() {return whList;}

 public:
  Server();
  ~Server() {}
  void run();
};

#endif
