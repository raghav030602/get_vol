//#include "/home/dev/StrategyBox/strategyConnector.h"
#include "strategy.h"

std::shared_ptr<Strategy> _strategy;
StrategyConnector::StrategyConnector(StrategyManager* strategyManager_, std::string strategyConfigFilePath_)
{
	_strategyManager = strategyManager_;
	_strategy = std::make_shared<Strategy>(strategyConfigFilePath_);
	_strategy-> setStrategyConnector(this);
	 std::vector<std::string> ABC={"SBIN_EQ"};
	_strategy->onStrateConnectionCallBack();
	

}

void StrategyConnector::onMarketData(std::string symbol,int32_t open,int32_t high,int32_t low ,int32_t close,int64_t volume,int32_t ltp,int32_t ltq,int32_t ltt, int32_t atp,int32_t lot, std::string expiry,int32_t bidPriceArray[5],int64_t bidQuantityArray[5],int32_t askPriceArray[5],int64_t askQuantityArray[5],int32_t lastUpdateTime){
  _strategy->updateData(symbol, open, high, low, close, volume, ltp, ltq, ltt, atp, lot, expiry, bidPriceArray, bidQuantityArray, askPriceArray, askQuantityArray, lastUpdateTime);
   

}

void StrategyConnector::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
	//_strategy->onResponse(responseType, orderID, symbol, side, openQuantity, price, errorCode, orderType, fillQuantity);
	_strategy->onResponse(responseType, errorCode, orderID, symbol, side, openQuantity, fillQuantity, price, orderType);
}
void StrategyConnector::onMarketData(std::string symbol,uint8_t messageType, long long int time , uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10],double  orderId1,double  orderId2){
     //	_strategy->updateData(symbol, messageType, time,side, price, quantity, expiry, strike, lot, callput, level,orderId1,orderId2);
}

void StrategyConnector::onMarketData(std::string symbol,uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10]){
    // std::cout<<"data:"<<symbol<<std::endl;
   _strategy->updateData(symbol, messageType, time, side, price, quantity, expiry, strike, lot, callput, level);
}

void StrategyConnector::onMarketData(std::string symbol, std::string date,long long int time,int open,int high, int low, int close, int64_t volume,int OI, int lot, std::string expiry, std::string callput)
{
	_strategy->updateData(symbol, date, time, open, high, low, close, volume, OI, lot, expiry, callput);
	_strategy->checkSignal(symbol);
	_strategy->checkSL(symbol);
}
		//void send(char side, int qty, int px, string symbol, char orderType,long long orderID, bool matchRemainingInNeXtBarOverRide, int fillQuantity=0);
//void StrategyConnector::send(char side, int qty, int px, string symbol, char orderType,long long orderId, bool nextBarMatching, int fillQuantity)
void StrategyConnector::send(char side, int qty, int px,std::string symbol, char orderType,long long orderId, bool matching, int fillQuantity, int disclosedQuantity, int iocFlag)
{
	//_strategyManager->sendOrder(side, qty, px, symbol, orderType, orderId, nextBarMatching, fillQuantity);
	_strategyManager->sendOrder(side, qty, px, symbol, orderType, orderId,matching, fillQuantity, disclosedQuantity, iocFlag);
}

// void StrategyConnector::sendOrderMultiLeg(TCP_ORDER_MULTILEG &tcpOrderMultileg){
//      _strategyManager->sendOrderMultiLeg(tcpOrderMultileg);
// }
void StrategyConnector::subscribeToSymbol(std::string symbol)
{
	_strategyManager->subscribeToSymbol(symbol);
}

void StrategyConnector::currMinuteBarsFinished()
{
	_strategy->currMinuteBarsFinished();
}
void StrategyConnector::processTimerEvent()
{
	_strategy->timerEvent();
}
void StrategyConnector::processTopOfBook(std::string symbol,BookLevel records[])
{
	_strategy->processTopOfBook(symbol,records);
}

void StrategyConnector::signalCallback(int signal)
{
	_strategy->signalCallback(signal);

}

void StrategyConnector::subscribeAndRecoverBookSymbols (std::vector<std::string> &solaceSubsSymList){
	  _strategyManager -> subscribeAndRecoverBookSymbols(solaceSubsSymList);
}

void StrategyConnector::getTopOfBook(std::string symbol) {
       _strategyManager->getTopOfBook(symbol);
}

int32_t StrategyConnector::getLastestOrderId(int32_t orderId) {
	return _strategyManager->getLastestOrderId(orderId);
}

int32_t StrategyConnector::getOriginalOrderId(int32_t orderId) {
	return _strategyManager->getOriginalOrderId(orderId);
}

void StrategyConnector::clearOrderRetryQueue() {
	_strategyManager->clearOrderRetryQueue();
}

long long StrategyConnector::getOrderId(){
	return _strategyManager->getOrderId();
}
