#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
# include <Windows.h>
# define sleep( seconds) Sleep( seconds * 1000);
#else
# include <unistd.h>
#endif



#include "interface.h"

#include "TestCppClient.h"

#include "rapidjson/rapidjson.h"  
#include "rapidjson/document.h"  
#include "rapidjson/reader.h"  
#include "rapidjson/writer.h"  
#include "rapidjson/stringbuffer.h"  

using namespace rapidjson;
using namespace std;


char gHost[128] = "127.0.0.1";
int gPort = 4001;

bool gIsOpen = false;
TestCppClient gClient;


int Test(char *echo) {

	const char* result = "VER20191219001";
	memcpy(echo, result, strlen(result));

	return (int)strlen(result);
}

bool InitMarket() {
	return false;
}

bool InitTrade() {

	int clientId = 0;
	gIsOpen = true;

	while(gIsOpen) {
		gClient.connect(gHost, gPort, clientId);
		while (gClient.isConnected()) {
			gClient.processMessages();
			if (!gIsOpen) {
				printf("交易已经关闭\n");
				break;
			}
		}
		printf("连接失败，稍后尝试\n");
		sleep(10);
	}

	printf("退出主进程\n");
	return true;
}


bool CloseMarket() {
	return false;
}
bool CloseTrade() {
	printf("关闭交易");
	gIsOpen = false;
	return true;
}
bool Config(char *config) {

	printf("配置参数...\n");

	if (config == NULL)
	{
		printf("无效参数...\n");
		Contract a50;
		a50.symbol = "XINA50";
		a50.secType = "Fut";
		a50.exchange = "SGX";
		a50.currency = "USD";
		a50.lastTradeDateOrContractMonth = "202001";

		Contract nifty;
		nifty.symbol = "NIFTY";
		nifty.secType = "Fut";
		nifty.exchange = "SGX";
		nifty.currency = "USD";
		nifty.lastTradeDateOrContractMonth = "202001";

		gClient.addInstrument(a50);
		gClient.addInstrument(nifty);
		return false;
	}

	if (strlen(config) == 0) {

	}
	else {
		Document doc;
		doc.Parse(config);
		
		Value& hostaddr = doc["host"];
		if (hostaddr != NULL) {
			const char * addr = hostaddr.GetString();
			memset(gHost, 0, 128);
			memcpy(gHost, addr, strlen(addr));
		}
		printf("配置服务器IP地址:%s\n", gHost);

		Value& port = doc["port"];
		if (port != NULL) {
			gPort = port.GetInt();
		}

		printf("配置服务器端口:%d\n", gPort);

		Value& instruments = doc["instruments"];
		if (instruments != NULL && instruments.IsArray()) {

			int length = instruments.Size();

			for (unsigned int i = 0; i < length; i++) {
				Value & v = instruments[i];
				Contract contract;

				contract.symbol = v["symbol"].GetString();
				contract.secType = v["sectype"].GetString();
				contract.exchange = v["exchange"].GetString();
				contract.currency = v["currency"].GetString();
				contract.lastTradeDateOrContractMonth = v["expire"].GetString();

				gClient.addInstrument(contract);
			}
		}
	}




	return 0;
}
int GetDepth(char *name, char *value) {
	return gClient.getDepth(name, value);
}
int GetKlines(char *name, int duration, char *value) {
	printf("GetKlines for %s\n", name);
	if (gClient.setCommand()) {
		int result = gClient.getKlines(name, duration, value);
		gClient.clearCommand();
		return result;
	}

	return 0;
}
int GetInstrumentInfo(char *name, char *info) {
	return gClient.getInstruments(info);
}
int GetPositionInfo(char *name, char *info) {
	printf("GetPositionInfo...\n");
	if (gClient.setCommand()) {
		int result = gClient.getPositions(info);
		gClient.clearCommand();
		return result;
	}
	return 0;
}
int GetBalance(char *info) {
	printf("GetBalance...\n");
	if (gClient.setCommand()) {
		int result = gClient.getBalance(info);
		gClient.clearCommand();
		return result;
	}
	return 0;
}
int MarketOpenPosition(char *instrumentID, int volume, int limitPrice, int isBuy, int isMarket, char *result) {
	printf("MarketOpenPosition...\n");
	if (gClient.setCommand()) {
		bool bBuy = false;
		if (isBuy > 0) {
			bBuy = true;
		}
		int len = gClient.placeOrder(instrumentID, bBuy, volume, result);
		gClient.clearCommand();
		return len;
	}
	return 0;
}
int MarketClosePosition(char *instrumentID, int volume, int limitPrice, int isBuy, int isMarket, int isToday, char *result) {
	return 0;
}
int MarketStopPrice(char *instrumentID, int volume, int isBuy, double stopPrice, double limitPrice, char *result) {
	return 0;
}
int CancelOrder(char *instrumentID, char *exchangeID, char *orderSysID, char *result) {
	return 0;
}
int GetStatus() {
	return 0;
}