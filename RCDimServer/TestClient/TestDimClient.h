/*
 * TestDimClient.h
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#ifndef TESTDIMCLIENT_H_
#define TESTDIMCLIENT_H_
#include "dic.hxx"
#include <string>
#include <vector>

using namespace std;


class TestDimClient : public DimClient{
public:
	enum {kIDLE, kINITIALIZED, kREADY};

	TestDimClient(string name);
	virtual ~TestDimClient();

	void infoHandler();

	void initialize();
	void startrun();
	void endrun();
	void reset();
	void requestConfig();
	int getDeviceState() const;

	void test();

	void sendFile(string fileName);
private:
	enum {kSTARTED, kSENDING, kWAITING, kENDED};
	void handleState(int i);
	void handleInfo(string s);
	void handleLogging(string s);
	//void handleWaiting(int i);
	void handleConfig(string s);

	//void sendNextFile();
	//void endTransfer();
	//void resetTransfer();

	//void modifyTransferStatus(int st);

	void print(string s);

	DimInfo *infoState;
	DimInfo *infoInfo;
	DimInfo *infoLogging;
	//DimInfo *infoWaiting;
	DimInfo *infoConfig;

	string dimServerName;

	//vector<string> files;
	//unsigned int currentFile;
	string coldFileName;
	string warmFileName;

	//int transferStatus;

	int deviceState;
};

#endif /* TESTDIMCLIENT_H_ */
