#ifndef _WAREHOUSE_H
#define _WAREHOUSE_H

#include<iostream>
using namespace std;

class Warehouse {
 private:
  int x;
  int y;
  int id;

 public:
  Warehouse(int loc_x, int loc_y, int ID) : x(loc_x), y(loc_y), id(ID) {}
  ~Warehouse() {}
  int getX() { return x; }
  int getY() { return y; }
  int getID() { return id; }
  void show(){
    cout<<"warehouse id: "<<id<<" loc_x: "<<x<<" loc_y: "<<y<<endl;
  }
};

#endif