#ifndef __STRATEGYMANAGER_USER__
#define __STRATEGYMANAGER_USER__

#include <unordered_set>
#include <string>
#include<vector>
//#include "structs.h"

extern std::unordered_set<std::string> validSymbols;
extern int32_t ModeWanted;
extern long long OrderIDStartingNumber;


struct BookLevel
{
	int32_t _quantity;
	int32_t _price;
	int16_t _noOfOrders;
	int16_t _buySellFlag;
}__attribute__((packed));

class StrategyManager
{
	public:
    void getTopOfBook(std::string);
    void sendOrder(char side, int qty, int px, std::string symbol, char orderType, long long orderId, bool val, int fillQuantity, int disclosedQuantity, int iocFlag);
    int32_t getLastestOrderId(int32_t orderId);
	int32_t getOriginalOrderId(int32_t orderId);
	void clearOrderRetryQueue();
    void subscribeToSymbol(std::string symbol);
	void subscribeAndRecoverBookSymbols(std::vector<std::string> &solaceSubsSymList);
	long long  getOrderId();


};
#endif
