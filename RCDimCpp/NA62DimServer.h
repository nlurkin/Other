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
	int getWaiting() const;
	void setWaiting(int waiting);

private:
	string dimServerName;

	//States: 0=IDLE, 1=INITIALIZED, 2=READY, else ERROR
	int state, waiting;
	char *info;
	char *logging;
	char *config;

	int infoIndex;

	DimService *dimState;
	DimService *dimInfo;
	DimService *dimLogging;
	DimService *dimWaiting;
	DimService *dimConfig;

	Command *dimCommand;
	FileContent *dimFileContent;
	EndTransfer *dimEndTransfer;

	int runNumber;
};


#endif /* NA62DIMSERVERX_H_ */
