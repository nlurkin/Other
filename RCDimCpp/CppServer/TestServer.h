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
		FileContent(dimServerName, (NA62DimServer*)parent),
		p(parent){};
	virtual ~TestFileContent(){};

private:
	void decodeFile(std::string fileContent);

	ConfigDecoder fDecoder;
	TestServer *p;
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
