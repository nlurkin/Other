/*
 * NA62DimCommands.h
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#ifndef NA62DIMCOMMANDS_H_
#define NA62DIMCOMMANDS_H_

#include <string>
#include "dis.hxx"
#include <vector>
#include "ConfigDecoder.h"
using namespace std;

class NA62DimServer;
enum FSMState {kIDLE=0, kINITIALIZED=1, kREADY=2};
//Example error states
enum ErrorState {kHARDWAREFAILURE=-10, kWRONGCONFIG=-11, kWRONGSTATE=-12, kUNKNOWN=-20};

class NA62DimCommand: public DimCommand
{
public:
	NA62DimCommand(string dimServerName, string commandName, string commandType, NA62DimServer *parent):
		DimCommand((dimServerName + "/" + commandName).c_str(), commandType.c_str()),
		p(parent){};

private:
	NA62DimCommand();
	NA62DimCommand(const NA62DimCommand &c);

	virtual void commandHandler() = 0;

protected:
	NA62DimServer *p;
};

class Command: public NA62DimCommand
{
public:
	Command(string dimServerName, NA62DimServer *parent):NA62DimCommand(dimServerName, "Command", "C", parent){};
private:
	virtual void doInitialize(vector<string> tok);
	virtual void doStartRun(vector<string> tok);
	virtual void doEndRun(vector<string> tok);
	virtual void doResetState(vector<string> tok);

	virtual void selectCommand(string commandName, vector<string> tok);
	void commandHandler();
};

class FileContent: public NA62DimCommand
{
public:
	FileContent(string dimServerName, NA62DimServer *parent):NA62DimCommand(dimServerName, "FileContent", "C", parent){};
private:
	void commandHandler();

	ConfigDecoder decoder;
};

class RequestConfig: public NA62DimCommand
{
public:
	RequestConfig(string dimServerName, NA62DimServer *parent):NA62DimCommand(dimServerName, "RequestConfig", "I", parent){};
private:
	void commandHandler();
};

#endif /* NA62DIMCOMMANDS_H_ */
