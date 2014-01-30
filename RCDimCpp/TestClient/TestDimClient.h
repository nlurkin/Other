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
	TestDimClient(string name);
	virtual ~TestDimClient();

	void infoHandler();

	void initialize();
	void startrun();
	void endrun();
	void reset();
private:
	void handleState(int i);
	void handleInfo(string s);
	void handleLogging(string s);
	void handleWaiting(int i);
	void handleConfig(string s);

	void sendFiles();

	DimInfo *infoState;
	DimInfo *infoInfo;
	DimInfo *infoLogging;
	DimInfo *infoWaiting;
	DimInfo *infoConfig;

	string dimServerName;

	vector<string> files;
};

#endif /* TESTDIMCLIENT_H_ */
