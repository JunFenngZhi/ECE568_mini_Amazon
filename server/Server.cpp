#include"Server.h"
#include"sql_function.h"


void Server::run(){
    cout << "initialize server...." << endl;
   
    // initialize database
    connection * C = connectDB("mini_amazon", "postgres", "passw0rd");
    initializeDB(C);
    disConnectDB(C);
    delete C;
    
    while (1)
    {
        
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
  connection * C = new connection("host=db port=5432 dbname=" + dbName + " user=" + userName + " password=" + password);  // use in docker
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