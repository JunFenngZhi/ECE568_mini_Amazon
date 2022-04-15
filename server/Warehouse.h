#ifndef _WAREHOUSE_H
#define _WAREHOUSE_H

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
};

#endif