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

class Command: public DimCommand
{
public:
	Command(string dimServerName, NA62DimServer *parent);
private:
	void doInitialize(vector<string> tok);
	void doStartRun(vector<string> tok);
	void doEndRun(vector<string> tok);
	void doResetState(vector<string> tok);

	void commandHandler();

	NA62DimServer *p;
};

class FileContent: public DimCommand
{
public:
	FileContent(string dimServerName, NA62DimServer *parent);
private:
	void commandHandler();

	NA62DimServer *p;
	ConfigDecoder decoder;
};

class EndTransfer: public DimCommand
{
public:
	EndTransfer(string dimServerName, NA62DimServer *parent);
private:
	void commandHandler();

	NA62DimServer *p;
};

#endif /* NA62DIMCOMMANDS_H_ */
