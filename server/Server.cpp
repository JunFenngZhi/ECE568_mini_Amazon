#include "Server.h"

#include <errno.h>

#include "protobufCommunication.hpp"
#include "socket.h"
#include "sql_function.h"

const int MAX_LENGTH = 65536;

/* ------------------------ "server initialize functions" ------------------------ */
Server::Server() {
  cout << "initialize server configuration...." << endl;
  worldID = 3;
  n_warehouse = 9;  // should be an odd number for symetric
  wh_distance = 20;
  webPortNum = "8888";
  worldHostName = "vcm-25941.vm.duke.edu";
  worldPortNum = "12345";
}

/* ------------------------ "server runtime functions" ------------------------ */
void Server::run() {
  try {
    initializeWorld();
    acceptOrderRequest();
  }
  catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return;
  }
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
  order request, send it to Request queue for processing.
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
    int len = MAX_LENGTH;
    vector<char> buffer(len, 0);
    recvMsg(client_fd, &(buffer.data()[0]), len);
    string orderRequest(buffer.data(), len);
    send(client_fd, "ACK", 4, 0);
    cout << "successfully receive order request.\n";
    cout << orderRequest.c_str() << endl;

    // TODO: put request into queue.
    close(client_fd);
  }
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