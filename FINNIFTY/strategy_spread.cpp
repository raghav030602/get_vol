#include "strategy_spread.h"
#include <ctime>
#include "blackScholes-orignal.cpp"
#include <bits/stdc++.h>
int counter = 0;
//double vol_final=0.0;


Strategy::Strategy(std::string filepath)                       //2nd config file path will go as input.
{
	std::ifstream file(filepath);
	 if (!file.is_open()) {
            std::cerr << "Error opening file: " << filepath << std::endl;
            return; // Exit the constructor if the file cannot be opened
        }
	
	std::string line;
	while (std::getline(file, line)) {
		std::size_t equal_pos = line.find('=');
		if (equal_pos == std::string::npos) {
		std::cerr << "Invalid format in line: " << line << std::endl;
		 continue;
}
		std::string key = line.substr(0, equal_pos);
		std::string value = line.substr(equal_pos + 1);
		 key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
		value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
		//std::istringstream iss(line);
		//std::string key, equal_sign, value;
		
		//if (!(iss >> key >> equal_sign >> value) || equal_sign != "=") {
            	//	std::cerr << "Invalid format in line: " << line << std::endl;
            	//	continue; // Skip invalid lines
        	//}

		if (key == "symbol1") {
            		symbol1 = value;
        	}
		else if (key == "strike_gap") {
            		strike_gap = std::stoll(value);
		}
		else if (key == "strike_level") {
            		strike_level = std::stoi(value); 
        	}
		else if (key == "weekely_expiry") {
            		weekely_expiry = std::stoll(value); 
        	} 
	}
	file.close();
}


string Strategy:: get_curr_time(){                  //HH:MM:SS
	std::time_t now=std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	std::ostringstream timeStream;
	timeStream << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0') << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0') << localTime->tm_sec;
	return timeStream.str();
}


string Strategy:: get_curr_time2() {                    //HH MM SS
    // Get the current system time
     auto now = std::chrono::system_clock::now();
     std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
     std::tm local_tm = *std::localtime(&now_time_t);
     std::ostringstream oss;
     oss << std::setw(2) << std::setfill('0') << local_tm.tm_hour << " "<< std::setw(2) << std::setfill('0') << local_tm.tm_min << " "<< std::setw(2) << std::setfill('0') << local_tm.tm_sec;
    
     return oss.str();
 }

std::vector<std::string> Strategy:: splitString(std::string str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
               tokens.push_back(token);
        }
        return tokens;
}
//vector<string>FUT_info=splitString(symbol1 , '_');

string Strategy:: get_curr_date() {                //YYYYMMDD
	//Get the current time as a time_t object
        time_t now = time(0);
        struct tm tstruct;
        tstruct = *localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tstruct, "%Y%m%d");
        return oss.str();
}


tm Strategy:: parseDate(const string& dateStr) {
    tm timeStruct = {};
    stringstream ss(dateStr);
    ss >> get_time(&timeStruct, "%Y%m%d");
    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;
    return timeStruct;
}

int Strategy::daysBetween(const string& date1, const string& date2) {             //Take dates in YYYYMMDD format and return number of days between them.
    tm tm_date1 = parseDate(date1);
    tm tm_date2 = parseDate(date2);

    time_t time1 = mktime(&tm_date1);
    time_t time2 = mktime(&tm_date2);

    // Calculate difference in seconds and convert to days
    double difference = difftime(time2, time1);
    return difference / (60 * 60 * 24);
}


string Strategy:: get_date_from_epoch(long long epoch_since_1980){                  // take epoch since 1980 and return data in YYYYMMDD format
        const long long seconds_between_1970_and_1980 = 315532800LL;
        long long actual_epoch = epoch_since_1980 + seconds_between_1970_and_1980;
        time_t epoch_time = static_cast<time_t>(actual_epoch);
        struct tm* time_info = localtime(&epoch_time);
        char buffer[9];
        strftime(buffer, sizeof(buffer), "%Y%m%d", time_info);
        return std::string(buffer);
}

double Strategy::spread_multiplier(){
	vector<string>FUT_info=splitString(symbol1 , '_');
	string weekely_date = get_date_from_epoch(weekely_expiry);
	long long temp = stol(FUT_info[1]);
	string monthly_date=get_date_from_epoch(temp);	
	string curr_date = get_curr_date();
	int y=daysBetween(weekely_date , monthly_date);
	int x=daysBetween(curr_date , monthly_date);
	return y/x;		
}

double Strategy:: get_spread(const std::string &filename){
	std::ifstream file(filename);
    	if (!file.is_open()) {
        	std::cerr << "Error opening file: " << filename << std::endl;
        	return -1;
    	}
	std::string line;
    	std::string currentDate = get_curr_date();
    	double spread = -1;
	
	while (std::getline(file, line)) {
        	std::istringstream iss(line);
        	std::string date;
        	std::string value;
		if(std::getline(iss, date, ',') && std::getline(iss, value)) {
        		if (date == currentDate) {
                		spread = std::stod(value);
                		break;
            		}
        	}
    	}	
	file.close();
    	return spread;
}


int Strategy:: get_curr_time3(){                //HHMMSS
	 std::time_t now = std::time(nullptr);
	std::tm *local_tm = std::localtime(&now);
	int formattedTime = (local_tm->tm_hour * 10000) + (local_tm->tm_min * 100) + local_tm->tm_sec;
	return formattedTime;
}

// symbol will contain complete info of option.
void Strategy:: updateData(std::string symbol,int32_t open,int32_t high,int32_t low ,int32_t close,int64_t volume,int32_t ltp,int32_t ltq,int32_t ltt, int32_t atp,int32_t lot, std::string expiry,int32_t bidPriceArray[5],int64_t bidQuantityArray[5],int32_t askPriceArray[5],int64_t askQuantityArray[5],int32_t lastUpdateTime) {
//	std::cout<<"Symbol::"<<symbol<<" Open:"<<open<<" High:"<<high<<" Low:"<<low<<" Close:"<<close<<" Vol:"<<volume<<"LTP:"<<ltp<<" LTQ:"<<ltq<<" LTT:"<<ltt<<" ATP:"<<atp<<" LOT:"<<lot<<" Expiry:"<<expiry<<" BB_PRICE:"<<bidPriceArray[0]<<" BA_price:"<<askPriceArray[0]<<" LastUpdateTime:"<<lastUpdateTime<<" check"<<std::endl;
    	counter++;
//	string spread_symbol = "NIFTYFINSERVICE";
//	unordered_map<string , long long >mp_spread;
	//if(mp_spread.find(spread_symbol)==mp_spread.end()){
//		subscribeToSymbol(spread_symbol);		
//	}
	//else{
//		if(symbol==spread_symbol){
//			mp_spread[spread_symbol]=ltp;
		//	return;	
		}
	//}	
	
	//for(auto it:mp_spread)cout<<it.first<<" "<<it.second<<endl;

//    	std::cout<<"counter:"<<counter<<std::endl;
	unordered_map<string , long long >symbol_to_LTP;
	unordered_map<string ,string>symbol_to_expiry;
	symbol_to_LTP[symbol]=ltp;
	symbol_to_expiry[symbol]=expiry;
	long long strike_gap=50; // Strike gap in Rs
    	//string time= get_curr_time();
	string curr_date = get_curr_date();
	std::string filename = "FINNIFTY_spot.csv";
	double spread_value1 = get_spread(filename);
    	//mp_option[symbol]=ltp - mp_spread[curr_date];
    	//mp_option[symbol]=ltp - 167*100;
//	long long weekely_expiry=1412433000;
	//long long temp=mp_option[symbol];
	vector<string>underlying=splitString(symbol , '_');
	if(underlying[2]!="FUT"){
		mp_option[symbol]=ltp;
	}
	//actual_LTP_FUT = ltp-167*100;
//	long long strike_gap_2=50;	
	//cout<<"1"<<endl;
	//cout<<temp<<endl;
	//temp/=10000;
	//temp*=10000;
	double spread_value;
//	double final_spread_multiplier = spread_multiplier();
//	double actual_spread=ltp-mp_spread[spread_symbol];
//	double spread_value2=final_spread_multiplier*actual_spread;
	//cout<<final_spread_multiplier <<" "<< to_string(actual_spread) <<" "<< to_string(spread_value2)<<endl;
	if(underlying[2]=="FUT"){
//		if(mp_spread.find(spread_symbol)==mp_spread.end()){
//			spread_value=spread_value1;
//		}
//		else{
//			spread_value=spread_value2;
//		}
		mp_option[symbol]=ltp - spread_value1;
		long long temp=mp_option[symbol];
//		cout<<"Temp = "<<temp<<endl;
		mp_option[symbol]=temp;
		temp/=10000;
		temp*=10000;
		for(int i=0;i<=10;i++){
//			cout<<temp<<endl;
			long long call_strike = temp + (i*strike_gap*100);
			long long put_strike = temp - (i*strike_gap*100);
		//	string call_expiry , put_expiry;
		//	if(symbol_to_expiry[call_option]==0){
		//		subscribeToSymbol(call_option);
		//	}
			
			string call_option = underlying[0] + '_' + to_string(weekely_expiry) + '_' + to_string(call_strike) + '_' + "CE";       // need to take weekely_expiry as input.
			string put_option = underlying[0] + '_' + to_string(weekely_expiry) + '_' + to_string(put_strike) + '_' + "PE";
			
//			cout<<call_option<<" "<<put_option<<endl;

			if(symbol_to_LTP.find(call_option)==symbol_to_LTP.end()){
				subscribeToSymbol(call_option);
			}
			else{
				mp_option[call_option]=symbol_to_LTP[call_option];
			}
			if(symbol_to_LTP.find(put_option)==symbol_to_LTP.end()){
				subscribeToSymbol(put_option);
			}
			else{
				mp_option[put_option]=symbol_to_LTP[put_option];			
			}	
		}
	}
	
//	int curr_time_to_check =  get_curr_time3();
//	if(curr_time_to_check > lastUpdateTime){
//		cout<<get_vol()<<endl;
//	}


	for(auto it:mp_option){
//		cout<<it.first <<" -> " << it.second<<endl;
}
	
//	cout<<"temp time = "<<temp_time<<endl;
	if(lastUpdateTime > temp_time){
		//cout<<get_vol()<<endl;
		get_vol();
		temp_time=lastUpdateTime;	

        }	
}


void Strategy::updateData(std::string symbol, uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel booklvl[10]) {
    // For demonstration, we print the received values
    std::cout << "Symbol: " << symbol <<" ";
    std::cout << "Message Type: " << static_cast<int>(messageType) <<" ";
    std::cout << "Time: " << time <<" ";
    std::cout << "Side: " << static_cast<int>(side) <<" ";
    std::cout << "Price: " << price <<" ";
    std::cout << "Quantity: " << quantity <<" ";
    std::cout << "Expiry: " << expiry <<" ";
    std::cout << "Strike: " << strike <<" ";
    std::cout << "Lot: " << lot <<" ";
    std::cout << "Call/Put: " << callput << std::endl;
    
 
  for (int i = 0; i < 10; ++i) {
       std::cout << "Record " << i + 1 << ": "
                  << "Quantity = " << booklvl[i]._quantity << ", "
                  << "Price = " << booklvl[i]._price << ", "
                  << "Number of Orders = " << booklvl[i]._noOfOrders << ", "
                  << "Buy/Sell Flag = " << booklvl[i]._buySellFlag << std::endl;
   }     
  
}


void Strategy::updateData(std::string symbol,uint8_t,long long int time,uint8_t side,long long int price,int quantity,std::string expiry,int strike,int lot,std::string callput,BookLevel booklvl[10], long long int orderId1, long long int orderId2){
       std::cout << "Symbol: " << symbol << " ";
        std::cout << "Time: " << time << " ";
        std::cout << "Side: " << static_cast<int>(side) << " ";
        std::cout << "Price: " << price << " ";
        std::cout << "Quantity: " << quantity << " ";
        std::cout << "Expiry: " << expiry << " ";
        std::cout << "Strike: " << strike << " ";
        std::cout << "Lot: " << lot << " ";
        std::cout << "Call/Put: " << callput << " ";

        std::cout << "OrderId1: " << orderId1 << " ";
        std::cout << "OrderId2: " << orderId2 << std::endl;

        for (int i = 0; i < 10; ++i) {
       std::cout << "Record " << i + 1 << ": "
                  << "Quantity = " << booklvl[i]._quantity << ", "
                  << "Price = " << booklvl[i]._price << ", "
                  << "Number of Orders = " << booklvl[i]._noOfOrders << ", "
                  << "Buy/Sell Flag = " << booklvl[i]._buySellFlag << std::endl;
    }
}

// Note : instead of OrderIDStartingNumber++ use getOrderId()
void Strategy::updateData(std::string symbol, std::string date,long long int time,int open,int high, int low, int close, int64_t volume,int OI, int lot, std::string expiry, std::string callput)
{
	std::cout << "Strategy : " << symbol << ' ' << date << ' ' << time << ' ' << open <<' ' << high << ' ' << low << ' ' << close << ' ' << volume << " OI:" << OI << ' ' << lot << ' ' << expiry << ' ' << callput << std::endl;
	// 	send('S', lot, close, symbol, 'N',OrderIDStartingNumber++, 0);
}

void Strategy::checkSignal(std::string symbol)
{

}

void Strategy::checkSL(std::string symbol)
{

}

void Strategy::onResponse(char responseType, int16_t errorCode, long long int orderID, std::string symbol, char side, int openQuantity,int fillQuantity,int price, char orderType)
{
    // std::cout << "responseType: " << responseType << " ";
    // std::cout << "errorCode: " << errorCode << " ";
    // std::cout << "orderID: " << orderID << " ";
    // std::cout << "symbol: " << symbol << " ";
    // std::cout << "side: " << side << " ";
    // std::cout << "openQuantity: " << openQuantity << " ";
    // std::cout << "fillQuantity: " << fillQuantity << " ";
    // std::cout << "price: " << price << " ";
    // std::cout << "orderType: " << orderType << std::endl;
}
void Strategy::setStrategyConnector(StrategyConnector *stc)
{
	connector = stc;
}

void Strategy::send(char side, int qty, int px, std::string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int disclosedQuantity,int ioc)
{
//	connector->send(side,qty,px,symbol,orderType,connector->getLastestOrderId(oid),matchRemainingInNeXtBarOverRide,fillQuantity,disclosedQuantity,ioc);
}


void Strategy::currMinuteBarsFinished()
{
	std::cout<<"GOT BAR FINISHED"<<std::endl;
}
void Strategy::signalCallback(int n)
{

    // send('B', 1, 12250, "PNB_1406385000_FUT", 'N',OrderIDStartingNumber++,false,1,1,0);
    // return;
	// TCP_ORDER_MULTILEG order;
    
    // // Initialize the main struct
    // order._code = 2;
    // order.messageLength = 0;
    // order._numberOfLegs = 2;
    // order._orderType = 'N'; // 'B' for Buy, for example
    // order._internalOrderId = getOrderId() + 2;
    // order._exchangeOrderId = 0;
    // order.status = 0; // status example
    // order.errorCode = 0; // no error
    // order.ioc = true; // Immediate or Cancel
    // order._lastReferenceActivity = 1627500000; // example timestamp
    // memset(order._reserved, 0, sizeof(order._reserved)); // Zero-initialize reserved field
    // std::string symb = "NIFTY_1406385000_FUT";
    // // Initialize the legs
    // for(int i = 0; i < 2; ++i) {
    //      memcpy(order._leg[i]._symbol, symb.c_str(), sizeof(order._leg[i]._symbol));
    //     order._leg[i]._price = 2257525; // Example fill price
    //     order._leg[i]._quantity =  5; // Example quantity
    //     order._leg[i]._disclosedQuantity = 1; // Example disclosed quantity
    //     order._leg[i]._side = 'B'; // Example side (0 or 1)
    //     order._leg[i]._fillQuantity = 0; // Example fill quantity
    //     order._leg[i]._openQuantity = 0; // Example open quantity
    //     memset(order._leg[i]._reserved, 0, sizeof(order._leg[i]._reserved)); // Zero-initialize reserved field
    // }


	// connector ->  sendOrderMultiLeg(order);
    // send('B', 1, 12250, "PNB_1406385000_FUT", 'N',OrderIDStartingNumber++,false,1,1,0);
    // pOfBook("SBIN_EQ");
     //getTopOfBook("SBIN_EQ");
     //getTopOfBook("SBIN_20240926_FUT");
          
    getTopOfBook("NIFTY_20240926_FUT");
}
void Strategy::timerEvent()
{
    std::cout<<"TIMER EVENT"<<std::endl;
}

void Strategy::processTopOfBook(std::string symbol,BookLevel records[])
{

  std::cout << "Symbol: " << symbol << std::endl;
    for (int i = 0; i < 10; ++i) {
       std::cout << "Record " << i + 1 << ": "
                  << "Quantity = " << records[i]._quantity << ", "
                  << "Price = " << records[i]._price << ", "
                  << "Number of Orders = " << records[i]._noOfOrders << ", "
                  << "Buy/Sell Flag = " << records[i]._buySellFlag << std::endl;
    }

}
void Strategy::onStrateConnectionCallBack(){
	std::vector<std::string> v = {"HINDUNILVR_1409409000_FUT"};
	connector -> subscribeToSymbol(
        "NIFTY_1409409000_FUT"
    );
}

void Strategy::subscribeToSymbol(std::string symbol) {
	  connector->subscribeToSymbol(symbol);
//	cout<<"Subscribing " << symbol <<endl;
}

void Strategy::getTopOfBook(std::string symbol) {
     connector->getTopOfBook(symbol);
}

long long int Strategy::getOrderId() {

	return connector->getOrderId();
}

long long Strategy::getOriginalOrderId(long long int orderId) {
    return connector->getOriginalOrderId(orderId);
}

long long Strategy::getLatestOrderId(long long int orderId) {
	 return connector->getLastestOrderId(orderId);
}

void Strategy::TopOfBook(std::string symbol, BookLevel *book) {
	connector->getTopOfBook(symbol);
}
/*
string Strategy:: get_curr_time() {
 //  
   auto now = std::chrono::system_clock::now();
   std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
   std::tm local_tm = *std::localtime(&now_time_t);
   std::ostringstream oss;
   oss << std::put_time(&local_tm, "%H:%M:%S");
 /  return oss.str();
}
*/

long long Strategy:: help(long long val , long long strike) {
	val/=100;
 	if (static_cast<long long >(val) % strike == 0) {
        	return (val + strike)*100;
        }
        return (static_cast<long long >(val) / strike + 1) * 100*strike;
}


double Strategy:: cal_vol(BlackScholes &obj_ce , BlackScholes &obj_pe , double val1 , double val2){
	double vol1=obj_ce.getImpliedVol_NewtonRaphson(val1 , 'c');
	double vol2=obj_pe.getImpliedVol_NewtonRaphson(val2 , 'p');

//	bool is_vol1_valid = !std::isnan(vol1);
//	bool is_vol2_valid = !std::isnan(vol2);
	
	cout<<"vol1 = " <<vol1<<" "<<"vol2 = "<<vol2<<endl;
	//cout<<val1<<" "<<val2<<endl;
	/* 
	if(vol1!=0 && vol2!=0){
		return (vol1 + vol2)/2.0;	
	}
	else if(vol1!=0){
		return vol1;
	}
	else if(vol2!=0){
		return vol2;
	}
	else{
		return 0.0;
	}
*/
return (vol1 + vol2)/2.0;
	
}


 long long int Strategy::get_epoch_since_1980(const std::string& date, const std::string& time) {
                string dateTimeStr = date + " " + time;
                struct tm tm = {0};
                istringstream ss(dateTimeStr);
                ss >> get_time(&tm, "%Y%m%d %H %M %S");
                if (ss.fail()) {
                        cout << "Incorrect date format: " << dateTimeStr << std::endl;
                        return -1;
                }
                time_t epochTime = mktime(&tm);
                struct tm tm_1980 = {0};
                tm_1980.tm_year = 1980 - 1900;
                tm_1980.tm_mon = 0;
                tm_1980.tm_mday = 1;
                tm_1980.tm_hour = 0;
                tm_1980.tm_min = 0;
                tm_1980.tm_sec = 0;

                time_t epoch1980 = mktime(&tm_1980);

                return static_cast<long long int>(epochTime - epoch1980);
}


double Strategy:: epoch_to_years(long long epoch1, long long epoch2) {

		const double seconds_in_a_year = 365.25 * 24 * 60 * 60;
		long long time_difference = std::llabs(epoch1 - epoch2);
		return time_difference / seconds_in_a_year;
}

 double Strategy:: expiry_time(string s , string date , string time){
                long long temp_2 = get_epoch_since_1980(date,time);
                long long temp_1 = stol(s);
                return epoch_to_years(temp_2 , temp_1);
 }


double Strategy:: getMultiplier(){
	time_t now = time(0);
	tm* curr_time = localtime(&now);
	tm target_time = *curr_time;
	target_time.tm_hour = 15;
    	target_time.tm_min = 30;
    	target_time.tm_sec = 0;
	time_t target_seconds = mktime(&target_time);
    	time_t curr_seconds = mktime(curr_time);
	double time_difference_seconds = difftime(target_seconds, curr_seconds);
	double time_difference_days = time_difference_seconds / 86400.0;
	if (time_difference_days < 0) {
        	time_difference_days = 0;
    	}
	return std::sqrt(time_difference_days);
}
/*
string Strategy:: get_date_from_epoch(long long epoch_since_1980){                  // take epoch since 1980 and return data in YYYYMMDD format
	const long long seconds_between_1970_and_1980 = 315532800LL;
	long long actual_epoch = epoch_since_1980 + seconds_between_1970_and_1980;
	time_t epoch_time = static_cast<time_t>(actual_epoch);
	struct tm* time_info = localtime(&epoch_time);
	char buffer[9];
	strftime(buffer, sizeof(buffer), "%Y%m%d", time_info);
	return std::string(buffer);
}
*/
long long Strategy:: get_epoch_1430(){
	std::time_t now = std::time(nullptr);
	std::tm local_tm = *std::localtime(&now);
	local_tm.tm_hour = 14;
	local_tm.tm_min = 30;
	local_tm.tm_sec = 0;
	std::time_t target_epoch = std::mktime(&local_tm);
	 return static_cast<long long>(target_epoch);
}

double Strategy:: get_vol(){
	double rate = 0.0;
	double dividends = 0.0;
	double vol =1.0;
	vector<string>temp_position=splitString(symbol1 , '_');
	strike_gap*=strike_level;
	string curr_time=get_curr_time();
	long long base_strike_price=mp_option[symbol1];
	//long long base_strike_price = actual_LTP_FUT;
//	base_strike_price/=10000;
//	base_strike_price*=10000;
	
	long long call_strike_price_1 = help(base_strike_price , strike_gap);
	long long put_strike_price_1 = call_strike_price_1 - (strike_gap*100);
//	cout<<"Initail Base = "<<base_strike_price<< " " << "Initial call = "<<call_strike_price_1<<" "<<"Initial Put = "<<put_strike_price_1<<endl;
	string time=get_curr_time2();
	string date=get_curr_date();
	string date_from_epoch=get_date_from_epoch(weekely_expiry);
	double time_to_expiry_in_epoch = expiry_time(to_string(weekely_expiry) , date , time);
	BlackScholes b_option_ce(double(call_strike_price_1) , double(base_strike_price) , rate , dividends , vol , time_to_expiry_in_epoch );
	BlackScholes b_option_pe(double(put_strike_price_1) , double(base_strike_price) , rate , dividends , vol , time_to_expiry_in_epoch );
	base_strike_price/=10000;
	base_strike_price*=10000;
	long long call_strike_price = help(base_strike_price , strike_gap);
	long long put_strike_price = call_strike_price - (strike_gap*100);
	string s_ce= temp_position[0] + "_" + to_string(weekely_expiry) + "_" + to_string(call_strike_price) + "_" + "CE";
	string s_pe= temp_position[0] + "_" + to_string(weekely_expiry) + "_" + to_string(put_strike_price) + "_" + "PE";
	double val_1 = stod(to_string(mp_option[s_ce]));
	double val_2 = stod(to_string(mp_option[s_pe]));
//	cout<<"val 1 = "<<mp_option[s_ce]<<endl;
//	cout<<"val 2 = "<<mp_option[s_pe]<<endl;
	double vol1=b_option_ce.getImpliedVol_NewtonRaphson(val_1 , 'c');
        double vol2=b_option_pe.getImpliedVol_NewtonRaphson(val_2 , 'p');
//	cout<<" vol 1 ="<<vol1<<" vol 2 ="<<vol2<<endl;		
	double vol_final = (vol1 + vol2)/2.0;
	double multiplier_factor = getMultiplier();
	if(date==date_from_epoch)vol_final*=multiplier_factor;
	//double vol_final = cal_vol(b_option_ce , b_option_pe , val_1 , val_2);			
	//cout<< vol_final<<endl;
//	cout<<"Final Base = "<<base_strike_price <<" "<< "Final Call = "<< call_strike_price<<" " <<"Final Put = " <<put_strike_price <<" " <<"Expiry Time = " <<time_to_expiry_in_epoch <<endl; 
//	cout<<"call symbol = "<<s_ce <<endl;
//	cout<<"put symbol = "<<s_pe<<endl;

	cout<<get_curr_time3()<<" "<<s_ce<<" "<<vol1<<" "<<s_pe<<" "<<vol2<<" "<<vol_final<<endl;
	return vol_final;	
	//cout<<"2"<<endl;
}


