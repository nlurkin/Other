/*
 * commands.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */

//TODO review do... commands

#include "helper.h"
#include "commands.h"
#include "server.h"
#include "dis.h"
#include <stdlib.h>
#include <string.h>

void stdDimCommand(long* tag, char* cmnd_buffer, int*size){
	print("Command port receiving: ");
	println(cmnd_buffer);

	char args[5][STRING_MAX_LENGTH];

	if(tokenize(args, cmnd_buffer, " ")>0){
		selectCommand(args[0], args);
	}
}

int stdSelectCommand(char commandName[STRING_MAX_LENGTH], char tok[5][STRING_MAX_LENGTH]){
	if(strcmp(commandName, "initialize")==0) doStdInitialize(tok);
	else if(strcmp(commandName, "startrun")==0) doStdStartRun(tok);
	else if(strcmp(commandName, "endrun")==0) doStdEndRun(tok);
	else if(strcmp(commandName, "resetstate")==0) doStdResetState(tok);
	else return -1;

	return 0;
}

void doStdInitialize(char tok[5][STRING_MAX_LENGTH]){
	if(fState==kIDLE || fState==kINITIALIZED){
		//If in one off the right state, wait for a config file
		println("Initializing");
		waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		println("Device is not in IDLE or INITIALIZED state. Cannot initialize.");
		setState(kWRONGSTATE);
	}
}
void doStdStartRun(char tok[5][STRING_MAX_LENGTH]){
	if(fState==kINITIALIZED){
		//If in one off the right state, wait for a config file
		print("Starting run number ");
		println(tok[0]);
		setRunNumber(atoi(tok[0]));
		waitConfigurationFile(kREADY);
	}
	else{
		//Go to error
		println("Device is not in INITIALIZED state. Cannot start a run.");
		setState(kWRONGSTATE);
	}

}
void doStdEndRun(char tok[5][STRING_MAX_LENGTH]){
	//If in one off the right state, wait for a config file
	if(getState()==kREADY){
		print("Stopping current run (");
		printi(fRunNumber);
		println(")");
		waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		println("Device is not in READY state. Cannot stop a run.");
		setState(kWRONGSTATE);
	}
}
void doStdResetState(char tok[5][STRING_MAX_LENGTH]){
	println("Reset requested");
	waitConfigurationFile(kIDLE);
}

void stdDimFileContent(long* tag, char* cmnd_buffer, int*size){
	int success;

	print("FileContent port receiving: ");
	println(cmnd_buffer);

	//Try to decode the file. If successful, try to apply the configuration to the parent server
	if(parseFile(cmnd_buffer, fConfigStruct)==0) success = applyConfiguration();
	else success = -1;

	if(success==0){
		//Successful, move to next expected state
		println("Finished processing config file... Moving to next state.");
		moveToExpectedState();
	}
	else {
		//Move to error
		println("Error while applying configuration.");
		setState(kCONFIGERROR);
	}
}

void stdDimRequestConfig(long* tag, char* cmnd_buffer, int*size){
	print("RequestConfig port receiving: ");
	printlni(*cmnd_buffer);

	if(*cmnd_buffer==1){
		publishConfig();

		dis_update_service(fDimConfig);
	}
	else{
		println("Unexpected value received from FileContent port.");
		setState(kUNKNOWN);
	}
}
