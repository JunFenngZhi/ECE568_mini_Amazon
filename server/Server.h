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
  vector<Warehouse> whList;  // list of warehouse
  int ups_fd;
  int world_fd;

 public:
  // ACK mechanism
  static vector<bool> seqNumTable;  // mark whether seqNum from amazon is acked.
  static size_t curSeqNum;  // the sequence number to be used next(add in send thread)
  static unordered_set<int>
      executeTable;  // mark whether specific response has been executed

  //IO thread
  static ThreadSafe_queue<ACommands>
      worldQueue;  // store the ACommands objects that need to be sent to the World.
  static ThreadSafe_queue<AUCommand>
      upsQueue;  // store the AUCommand objects that need to be sent to the UPS.

 private:
  void acceptOrderRequest();
  void initializeWorld();
  void getWorldIDFromUPS();
  void handleOrderRequest(string requestMsg);
  int selectWareHouse(const Order & order);
  void keepReceivingMsg();
  void keepSendingMsg();

 private:
  friend class AResponseHandler;

 public:  //如果使用线程池，放回private
  static connection * connectDB(string dbName, string userName, string W);
  static void initializeDB(connection * C);
  static void disConnectDB(connection * C);

 public:
  Server();
  ~Server() {}
  void run();
};

#endif

//TODO: 部分成员变量改为static?