#ifndef _SQL_FUNCTION_H
#define _SQL_FUNCTION_H

#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <string>

using namespace pqxx;
using namespace std;


void createTable(connection* C, const string fileName);
void dropAllTable(connection* C);


#endif