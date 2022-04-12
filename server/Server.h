#ifndef _SERVER_H
#define _SERVER_H

#include <iostream>
#include <pqxx/pqxx>
#include "exception.h"

using namespace std;
using namespace pqxx;

class Server {
 private:
  connection * connectDB(string dbName, string userName, string W);
  void initializeDB(connection * C);
  void disConnectDB(connection * C);

 public:
  Server() {}
  ~Server() {}
  void run();
};

#endif
