#include "UAResponseHandler.h"

#include "OrderProcess.h"

UAResponseHandler::UAResponseHandler(const UAResponse & r) {
    for (int i = 0; i < r.order_size(); i++) {
    aorderatrucks.push_back(std::move(r.order(i)));
    seqNums.push_back(r.order(i).seqnum());
  }
}