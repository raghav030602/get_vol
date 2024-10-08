
#define STRATEGY_H

#include "strategyConnectorUser.h"
#include <bits/stdc++.h>
#include <sstream>
#include <chrono>
#include<iomanip>
#include"blackScholes-orignal.cpp"
#include <vector>
#include <string>
#include <unordered_map>
//#include "structs.h"

class Strategy{
    public:
        Strategy(std::string);
        // MINUTELY
        void updateData(std::string symbol,std::string date,long long int time,int open,int high, int low, int close, int64_t volume,int OI, int lot, std::string expiry, std::string callput);
        // TBT
        void updateData(std::string symbol,uint8_t,long long int time,uint8_t side,long long int price,int quantity,std::string expiry,int strike,int lot,std::string callput,BookLevel booklvl[10], long long int orderId1, long long int orderId2);
        // #ifdef SimSnapshot
        // // SNAPSHOT
       void updateData(std::string symbol,int32_t open,int32_t high,int32_t low ,int32_t close,int64_t volume,int32_t ltp,int32_t ltq,int32_t ltt, int32_t atp,int32_t lot, std::string expiry,int32_t bidPriceArray[5],int64_t bidQuantityArray[5],int32_t askPriceArray[5],int64_t askQuantityArray[5],int32_t lastUpdateTime);
    //  void updateData(std::string smbl,int open,int high,int low,int close,float volume,int LTP,int LTQ,std::string LTT,int ATP,int lot,std::string expiry,std::vector<std::pair<int,int>> buyDepth,std::vector<std::pair<int,int>> sellDepth,long long int time);
        // #endif
        void updateData(std::string symbol,uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10]);
        void checkSignal(std::string);
        void checkSL(std::string);

        //void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
        void onResponse(char responseType, int16_t errorCode, long long int orderID, std::string symbol, char side, int openQuantity,int fillQuantity,int price, char orderType);
        void setStrategyConnector(StrategyConnector *stc);
        void send(char side, int qty, int px, std::string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int disclosedQuantity,int ioc);
        void currMinuteBarsFinished();

        void TopOfBook(std::string, BookLevel *);
        void timerEvent();
        //By Som
        void processTopOfBook(std::string symbol, BookLevel booklvl[]);
        void getTopOfBook(std::string symbol);
        long long int getOrderId();
        long long getOriginalOrderId(long long int);
        long long getLatestOrderId(long long int);
        void signalCallback(int n);
        void subscribeToSymbol(std::string symbol);
        void onStrateConnectionCallBack();
        void calculateNIFTY50Index(long long int time, BookLevel level[10], int lot, std::string symbol);
        void calculateNIFTYBANKIndex(long long int time, BookLevel level[10], int lot, std::string symbol);
        void loadContractSymbol();
        void loadPrevDayFile(const std::string find);
        void loadPrevDayFiles();
	//double get_vol(std::string symbol , long long strike_gap , int strike_level , long long weekely_expiry );
	double get_vol();
	double cal_vol(BlackScholes &obj1 , BlackScholes &obj2 , double val1 , double val2);
	long long help(long long val , long long strike);
	std::string get_curr_time();
	std::vector<std::string>splitString ( std::string s , char delimeter);
	std::string get_curr_date();
	std::string get_curr_time2();
	double expiry_time(std::string s , std::string date , std::string time);
	double epoch_to_years(long long epoch1 , long long epoch2);
	long long get_epoch_since_1980(const std::string& date, const std::string& time);
	int get_curr_time3();
	double getMultiplier();
	long long get_epoch_1430();
	double get_spread(const std::string &filename);
	string get_date_from_epoch(long long epoch);	
    private:
        StrategyConnector *connector;
        std::string stratconfigfilepath;
 	//map<string,unordered_map<string,long long>>mp_option;
 	std::unordered_map<std::string,long long > mp_option;
	std::unordered_map<std::string ,long long int>mp_spread;
	std::string symbol1;
	long long strike_gap;
	long long strike_level;
	long long weekely_expiry;
	int temp_time=0;
	long long actual_LTP_FUT;
	//double vol_final=0.0;
};
