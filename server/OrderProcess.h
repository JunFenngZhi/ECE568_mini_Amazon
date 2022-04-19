#ifndef _ORDER_PROCESS_H
#define _ORDER_PROCESS_H
#include "Server.h"
#include "sql_function.h"

void parseOrder(string msg);
int selectWareHouse(const Order & order);
void processOrder(const Order& order); 
void processPurchaseMore(APurchaseMore r);
void processPacked(APacked r);
void processLoaded(ALoaded r);
void pushWorldQueue(ACommands acommand, int seqNum);
void pushUpsQueue(AUCommand aucommand, int seqNum);
#endif
