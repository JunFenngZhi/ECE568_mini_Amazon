  #ifndef _ORDER_PROCESS_H
  #define _ORDER_PROCESS_H
  #include "Server.h"
  #include "sql_function.h"

  void processPurchaseMore(APurchaseMore r);
  
  void processPacked(APacked r);
  
  void processLoaded(ALoaded r);

  #endif
  