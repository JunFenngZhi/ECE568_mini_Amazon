#include "Server.h"
#include "protobufCommunication.hpp"

class AResponseHandler {
 private:
  vector<APurchaseMore> apurchasemores;
  vector<APacked> apackeds;
  vector<ALoaded> aloadeds;
  vector<int> seqNums;

 public:
  AResponseHandler(const AResponses & r);
  ~AResponseHandler() {}
  void handle();

 private:
  bool checkExecutedAndRecordIt(int seqNum);
};
