#ifndef _SERVER_H
#define _SERVER_H

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "./protobuf/world_amazon.pb.h"
#include "Warehouse.h"
#include "exception.h"

using namespace std;
using namespace pqxx;

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

 private:
  connection * connectDB(string dbName, string userName, string W);
  void initializeDB(connection * C);
  void disConnectDB(connection * C);
  void acceptOrderRequest();
  void initializeWorld();

 public:
  Server();
  ~Server() {}
  void run();
};

#endif
