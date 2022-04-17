#ifndef _SQL_FUNCTION_H
#define _SQL_FUNCTION_H

#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include "Order.h"

using namespace pqxx;
using namespace std;


void createTable(connection* C, const string fileName);
void dropAllTable(connection* C);
bool checkInventory(connection* C, int itemId, int itemAmount, int whID, int& version);
void addInventory(connection * C, int whID, int count, int productId);
void decreaseInventory(connection * C, int whID, int count, int productId, int version);
//TODO
string getDescription(connection * C, int itemId);
void saveOrderInDB(connection* C, const Order* order);
#endif