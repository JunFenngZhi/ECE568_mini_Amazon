#ifndef _ORDER_H
#define _ORDER_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  /*
    Parse the incoming orderInfo string from the front end to create the object.
  */
  Order::Order(const string & str) {
    //the order input string format is as following:
    //addrx:addry:amount:itemID:itemPrice:UPSID
    //res[0]:res[1]:res[2]:res[3]:res[4]:res[5]
    vector<string> res;
    stringstream input(str);
    string temp;
    const char pattern = ':';
    while (getline(input, temp, pattern)) {
      res.push_back(temp);
    }
    //Because the UPSID is optional, we need to judge if the UPSID is exists
    //if not exists, we set UPS_ID as -1 to indicate that
    if (res[5] == "") {
      UPS_ID = -1;
    }
    else {
      UPS_ID = stoi(res[5]);
    }
    address_x = stoi(res[0]);
    address_y = stoi(res[1]);
    amount = stoi(res[2]);
    item_id = stoi(res[3]);
    item_price = stof(res[4]);
  }
  int getAddressX() const { return address_x; }
  int getAddressY() const { return address_y; }
  int getAmount() const { return amount; }
  int getItemId() const { return item_id; }
  float getItemPrice() const { return item_price; }
  int getUPSId() const { return UPS_ID; }
};

#endif