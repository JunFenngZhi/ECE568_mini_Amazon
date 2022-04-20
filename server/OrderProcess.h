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
void pushWorldQueue(const ACommands& acommand, int seqNum);
void pushUpsQueue(const AUCommand& aucommand, int seqNum);
void packOrder(Order order);
void callATruck(Order order);
#endif
