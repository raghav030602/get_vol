#ifndef __STRATEGYCONNECTOR_H__
#define __STRATEGYCONNECTOR_H__
#include<string>
#include<memory>
#include<vector>

// #include "structs.h"
#include "strategyManagerUser.h"


class StrategyConnector
{
	public:
		StrategyManager* _strategyManager;
	StrategyConnector(StrategyManager* _strategyManager, std::string strategyConfigFilePath);
	void signalCallback(int n);
	void onResponse(char , long long int, std::string, char, int,int, int16_t, char,int);
    void onMarketData(std::string symbol,int32_t open,int32_t high,int32_t low ,int32_t close,int64_t volume,int32_t ltp,int32_t ltq,int32_t ltt, int32_t atp,int32_t lot, std::string expiry,int32_t bidPriceArray[5],int64_t bidQuantityArray[5],int32_t askPriceArray[5],int64_t askQuantityArray[5],int32_t lastUpdateTime);
	void onMarketData(std::string symbol, std::string date,long long int time,int open,int high, int low, int close, int64_t volume,int OI, int lot, std::string expiry, std::string callput);
	void onMarketData(std::string symbol,uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10],double  orderId1,double  orderId2);
	void onMarketData(std::string symbol,uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10]);	
	void send(char side, int qty, int px, std::string symbol, char orderType,long long orderID, bool matchRemainingInNeXtBarOverRide, int fillQuantity, int disclosedQuantity, int iocFlag);
	// void sendOrderMultiLeg(TCP_ORDER_MULTILEG &tcpOrderMultileg);
	void subscribeToSymbol(std::string symbol);
	void subscribeAndRecoverBookSymbols(std::vector<std::string> &solaceSubsSymList);
	int32_t getLastestOrderId(int32_t orderId);
	int32_t getOriginalOrderId(int32_t orderId);
	void clearOrderRetryQueue();
	void currMinuteBarsFinished();
	void getTopOfBook(std::string symbol); 
	void processTopOfBook(std::string symbol, BookLevel records[]);
	void processTimerEvent();
	long long  getOrderId();
};
#endif

