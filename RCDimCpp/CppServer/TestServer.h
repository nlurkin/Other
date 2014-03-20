/*
 * TestServer.h
 *
 *  Created on: Mar 19, 2014
 *      Author: nlurkin
 */

#ifndef TESTSERVER_H_
#define TESTSERVER_H_

#include "NA62DimServer.h"
#include "ConfigDecoder.h"

class TestServer: public NA62DimServer {
public:
	TestServer(std::string name, int sourceID);
	virtual ~TestServer();

	void mainLoop();
	void generateConfig(std::stringstream& ss);
	bool applyConfiguration();

	void setFrequency(double frequency);
	void setSourceId(int sourceId);
	void setUselessInt(int uselessInt);
	void setUselessString(const std::string& uselessString);
	double getFrequency() const;
	int getSourceId() const;
	int getUselessInt() const;
	const std::string& getUselessString() const;
	int getParam() const;
	void setParam(int param);

private:
	double fFrequency;
	int fSourceID;
	std::string fUselessString;
	int fUselessInt;
	int fParam;
};

class TestFileContent: public FileContent {
public:
	TestFileContent(std::string dimServerName, TestServer *parent):
		FileContent(dimServerName, (NA62DimServer*)parent){};
	virtual ~TestFileContent(){};

private:
	bool decodeFile(std::string fileContent, void* structPtr);

	ConfigDecoder fDecoder;
};

class TestCommand: public Command {
public:
	TestCommand(std::string dimServerName, TestServer *parent):
		Command(dimServerName, (NA62DimServer*)parent){};
	virtual ~TestCommand(){};

private:
	virtual void doEndRun(std::vector<std::string> tok);
	virtual void doResetState(std::vector<std::string> tok);
};
#endif /* TESTSERVER_H_ */
