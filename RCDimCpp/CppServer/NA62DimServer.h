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
#include <sstream>
using namespace std;

class NA62DimServer: public DimServer
{
public:
	NA62DimServer(string name);
	virtual ~NA62DimServer();

	void initCommands(Command *cmdCommand=NULL, FileContent *fileContentCommand=NULL, RequestConfig *requestConfigCommand=NULL);
	void start();

	//Printing to Info
	void print(const char *s);
	void print(string s);
	void print(int s);
	void println(const char *s);
	void println(string s);
	void println(int s);

	//virtual methods to implement
	virtual void mainLoop() = 0;
	virtual void generateConfig(stringstream& ss) = 0;

	//Mandatory run infos
	int getRunNumber() const;
	void setRunNumber(int runNumber);

	string getRunType() const {return fRunType;};
	void setRuntype(string runType) {fRunType = runType;};

	string getDimServerName() const {return fDimServerName;};

	void publishConfig();

	//State management
	int getState() const;
	void setState(int state);

	int getNextState() const;
	void setNextState(int nextState);

private:
	NA62DimServer();
	NA62DimServer(NA62DimServer&);
	NA62DimServer& operator=(NA62DimServer&);

	string fDimServerName;

	//States: 0=IDLE, 1=INITIALIZED, 2=READY
	// <0 : ERROR
	int fState;
	int fNextState;
	char *fInfo;
	char *fLogging;
	char *fConfig;

	int fInfoIndex;

	DimService *fDimState;
	DimService *fDimInfo;
	DimService *fDimLogging;
	DimService *fDimConfig;

	Command *fDimCommand;
	FileContent *fDimFileContent;
	RequestConfig *fDimRequestConfig;

	int fRunNumber;
	string fRunType;
};


#endif /* NA62DIMSERVERX_H_ */
