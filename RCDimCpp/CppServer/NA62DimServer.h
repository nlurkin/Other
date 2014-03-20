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

class NA62DimServer: public DimServer
{
public:
	NA62DimServer(std::string name, int sourceID);
	virtual ~NA62DimServer();

	void initCommands(Command *cmdCommand=NULL, FileContent *fileContentCommand=NULL, RequestConfig *requestConfigCommand=NULL);
	void start();

	//Printing to Info
	void print(const char *s);
	void print(std::string s);
	void print(int s);
	void println(const char *s);
	void println(std::string s);
	void println(int s);

	//Logging
	void centralizedLog(int severity, std::string text, int priority, int errCode=0);

	//virtual methods to implement
	virtual void mainLoop(){};
	virtual void generateConfig(std::stringstream& ss) = 0;
	virtual void applyConfig(void* structPtr) = 0;

	//Mandatory run infos
	int getRunNumber() const;
	void setRunNumber(int runNumber);

	std::string getRunType() const {return fRunType;};
	void setRuntype(std::string runType) {fRunType = runType;};

	std::string getDimServerName() const {return fDimServerName;};

	void publishConfig();

	//State management
	int getState() const;
	void setState(int state);

	void waitConfigurationFile(int expectedState);
	void moveToExpectedState();
private:
	NA62DimServer();
	NA62DimServer(NA62DimServer&);
	NA62DimServer& operator=(NA62DimServer&);

	void setNextState(int nextState);

	std::string fDimServerName;

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
	std::string fRunType;

	int fSourceID;
};


#endif /* NA62DIMSERVERX_H_ */
