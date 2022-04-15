#include "Server.h"

#include "protobufCommunication.hpp"
#include "socket.h"
#include "sql_function.h"

/* ------------------------ "server initialize functions" ------------------------ */
Server::Server() {
  cout << "initialize server configuration...." << endl;
  n_warehouse = 9;  // should be an odd number for symetric
  wh_distance = 20;
  webPortNum = "8888";
  worldHostName = "vcm-25941.vm.duke.edu";
  worldPortNum = "12345";
  upsHostName = "0.0.0.0";
  upsPortNum = "8888";

  //used in development
  worldID = 3;
}

/* ------------------------ "server runtime functions" ------------------------ */
void Server::run() {
  try {
    //getWorldIDFromUPS();
    initializeWorld();
    acceptOrderRequest();
  }
  catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return;
  }
}

/*
  Connect to UPS to get worldID. 
*/
void Server::getWorldIDFromUPS() {
  int ups_fd = clientRequestConnection(upsHostName, upsPortNum);
  string msg = recvMsg(ups_fd);

  worldID = stoi(msg);
  cout << "get worldID = " << worldID << " from UPS.\n";
  close(ups_fd);
}

/*
  Connect to a new world, and then initialize warehouses for amazon.
  If connect unsuccessfully, it will throw exception. If worldID = -1, 
  server will create a new world and set worldID to the new worldID.
*/
void Server::initializeWorld() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  //connect to world
  int world_fd = clientRequestConnection(worldHostName, worldPortNum);

  //set AConnect command
  AConnect ac;
  for (int i = 0; i <= n_warehouse / 2; i++) {
    if (i == 0) {
      AInitWarehouse * w = ac.add_initwh();
      w->set_id(i);
      w->set_x(0);
      w->set_y(0);
      whList.push_back(Warehouse(0, 0, i));
    }
    else {
      AInitWarehouse * w1 = ac.add_initwh();
      w1->set_id(i);
      w1->set_x(0.707 * wh_distance * i);
      w1->set_y(0.707 * wh_distance * i);
      whList.push_back(Warehouse(0.707 * wh_distance * i, 0.707 * wh_distance * i, i));

      AInitWarehouse * w2 = ac.add_initwh();
      w2->set_id(i + n_warehouse / 2);
      w2->set_x(-1 * 0.707 * wh_distance * i);
      w2->set_y(-1 * 0.707 * wh_distance * i);
      whList.push_back(
          Warehouse(-1 * 0.707 * wh_distance * i, -1 * 0.707 * wh_distance * i, i));
    }
  }
  if (worldID != -1) {
    ac.set_worldid(worldID);
  }
  ac.set_isamazon(true);

  //show warehouses
  for (auto & w : whList) {
    w.show();
  }

  //send AConnect command
  unique_ptr<socket_out> out(new socket_out(world_fd));
  if (sendMesgTo<AConnect>(ac, out.get()) == false) {
    throw MyException("fail to send AConnect to world.");
  }

  //receive AConnected command
  AConnected aced;
  unique_ptr<socket_in> in(new socket_in(world_fd));
  if (recvMesgFrom<AConnected>(aced, in.get()) == false) {
    throw MyException("fail to recv AConnected from world.");
  }

  //check AConnected command
  if (aced.result() != "connected!") {
    close(world_fd);
    throw MyException("fail to initialize the world.");
  }
  worldID = aced.worldid();
  cout << "succefully connect to world and initialize warehouses.\n";
  close(world_fd);
}

/*
  IO function. Server keep receiving order requests from front-end web.
  It will throw exception when server_socket create unsuccessfully. For each 
  order request, send it to task queue for processing.
*/
void Server::acceptOrderRequest() {
  // create server socket, listen to port.
  int server_fd = createServerSocket(webPortNum);

  // keep receving orders request from front-end web.
  while (1) {
    // wait to accept connection.
    int client_fd;
    string clientIP;
    try {
      client_fd = serverAcceptConnection(server_fd, clientIP);
    }
    catch (const std::exception & e) {
      std::cerr << e.what() << '\n';
      continue;
    }

    // receive request.
    string msg;
    try {
      msg = recvMsg(client_fd);
      sendMsg(client_fd, "ACK", 4);
      close(client_fd);
    }
    catch (const std::exception & e) {
      std::cerr << e.what() << '\n';
      continue;
    }

    // TODO: put request into task queue, using thread pool
    thread t(&Server::handleOrderRequest, this, msg);
    t.detach();
  }
}

/*
  handle order request from front-end web. function will connect to 
  world and ups server. It will achieve no-throw guarantee.
*/
void Server::handleOrderRequest(string requestMsg) {
  cout << "successfully receive order request.\n";
  cout << requestMsg.c_str() << endl;
  Order order(requestMsg);

  // determine to use which warehouse.
  int whIndex = selectWareHouse(order);

  // 检查该仓库的库存情况，是否满足订单 写在sql_function.cpp中
  

  // 否，则向world下单购买

  // 开始pack
}

/*
  select a warehouse, which is closest to the order address. return the selected warehouse index.
*/
int Server::selectWareHouse(const Order & order) {
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

/* ------------------------ "DB related functions" ------------------------ */
/*
  connect to specific database using the given userName and passWord, reutnr 
  the connection* C. It will throw an exception if fails. 
*/
connection * Server::connectDB(string dbName, string userName, string password) {
  //mtx_server.lock();
  //connection * C = new connection("dbname=" + dbName + " user=" + userName + " password=" + password);   // use in real sys
  connection * C =
      new connection("host=db port=5432 dbname=" + dbName + " user=" + userName +
                     " password=" + password);  // use in docker
  if (C->is_open()) {
    cout << "Opened database successfully: " << C->dbname() << endl;
  }
  else {
    throw MyException("Can't open database.");
  }
  //mtx_server.unlock();
  return C;
}

/*
  initialize database when server run at the first time. It will drop all the existing tables,
  and then create new ones.
*/
void Server::initializeDB(connection * C) {
  dropAllTable(C);
  createTable(C, "./sql/table.sql");
}

/*
  close the connection to database.
*/
void Server::disConnectDB(connection * C) {
  C->disconnect();
}