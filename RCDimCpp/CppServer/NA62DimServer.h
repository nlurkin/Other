/*
 * NA62DimServerx.h
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */
#ifndef NA62DIMSERVERX_H_
#define NA62DIMSERVERX_H_

#include <string>
#include "NA62DimCommands.h"
#include "dis.hxx"
using namespace std;

class NA62DimServer: public DimServer
{
public:
	NA62DimServer(string name);
	void start();

	void print(const char *s);
	void print(string s);
	void print(int s);
	void println(const char *s);
	void println(string s);
	void println(int s);

	void mainLoop();

	int getRunNumber() const;
	void setRunNumber(int runNumber);
	int getState() const;
	void setState(int state);
	void setFrequency(double frequency);
	void setSourceId(int sourceId);
	void setUselessInt(int uselessInt);
	void setUselessString(const string& uselessString);
	double getFrequency() const;
	int getSourceId() const;
	int getUselessInt() const;
	const string& getUselessString() const;
	int getParam() const;
	void setParam(int param);
	void setConfig(string config);
	int getNextState() const;
	void setNextState(int nextState);

private:
	string dimServerName;

	//States: 0=IDLE, 1=INITIALIZED, 2=READY, else ERROR
	int state;
	int nextState;
	char *info;
	char *logging;
	char *config;

	int infoIndex;

	DimService *dimState;
	DimService *dimInfo;
	DimService *dimLogging;
	DimService *dimConfig;

	Command *dimCommand;
	FileContent *dimFileContent;
	RequestConfig *dimRequestConfig;

	int runNumber;
	double frequency;
	int sourceID;
	string uselessString;
	int uselessInt;
	int param;
};


#endif /* NA62DIMSERVERX_H_ */
