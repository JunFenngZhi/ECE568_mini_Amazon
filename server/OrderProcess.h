  //APurchaseMore,创建新的线程, lend it to processPurchaseMore(去数据库中将对应warehouse中的对应product库存增加)
  void processPurchaseMore(APurchaseMore r);
  //APacked, 创建新的线程, lend it to processPacked(去数据库修改order status变为 ’packed‘)
  void processPacked(APacked r);
  //ALoaded, 创建新的线程, lend it to processLoaded(向UPS发送 AStartDeliver)
  void processLoaded(ALoaded r);