#ifndef _ORDER_H
#define _ORDER_H
#include<string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Order {
 private:
  int address_x;
  int address_y;
  int amount;
  int item_id;
  float item_price;
  int UPS_ID;

 public:
  Order() {}
  Order(const string& str);
  ~Order() {}
  int getAddressX() const { return address_x; }
  int getAddressY() const { return address_y; }
  int getAmount() const { return amount; }
  int getItemId() const {return item_id;}
  float getItemPrice() const {return item_price;}
  int getUPSId() const {return UPS_ID;}

};

#endif