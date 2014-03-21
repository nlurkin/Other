/*
 * server.c
 *
 *  Created on: Mar 21, 2014
 *      Author: nlurkin
 */

#include "server.h"
#include "helper.h"
#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include "dis.h"
#include <string.h>

int fDimState=0;
int fDimInfo=0;
int fDimLogging=0;
int fDimConfig=0;

int fRunNumber=0;

int fState=0;
int fNextState=-1;
char fInfo[STRING_MAX_LENGTH];
char fLogging[STRING_MAX_LENGTH];
char fConfig[CONFIG_MAX_LENGTH];

int fInfoIndex=0;
int fSourceID=0;

void initCommands(char* dimServerName, void (*commandRoutine)(void*,void*,int*), void (*fileContentRoutine)(void*,void*,int*), void (*requestConfigRoutine)(void*,void*,int*)){
	char commandName[CONFIG_MAX_LENGTH];
	char fileContentName[CONFIG_MAX_LENGTH];
	char requestConfigName[CONFIG_MAX_LENGTH];

	sprintf(commandName, "%s/%s", dimServerName, "Command");
	sprintf(fileContentName, "%s/%s", dimServerName, "FileContent");
	sprintf(requestConfigName, "%s/%s", dimServerName, "RequestConfig");

	if(commandRoutine==NULL) dis_add_cmnd(commandName, "C", stdDimCommand, 0);
	else dis_add_cmnd(commandName, "C", commandRoutine, 0);

	if(fileContentRoutine==NULL) dis_add_cmnd(fileContentName, "C", stdDimFileContent, 0);
	else dis_add_cmnd(fileContentName, "C", fileContentRoutine, 0);

	if(requestConfigRoutine==NULL) dis_add_cmnd(requestConfigName, "I", stdDimRequestConfig, 0);
	else dis_add_cmnd(requestConfigName, "I", requestConfigRoutine, 0);
}

void startServer(char* dimServerName, int sourceID){
	char stateName[STRING_MAX_LENGTH];
	char infoName[STRING_MAX_LENGTH];
	char loggingName[STRING_MAX_LENGTH];
	char configName[CONFIG_MAX_LENGTH];

	sprintf(stateName, "%s/%s", dimServerName, "State");
	sprintf(infoName, "%s/%s", dimServerName, "Info");
	sprintf(loggingName, "%s/%s", dimServerName, "Logging");
	sprintf(configName, "%s/%s", dimServerName, "Config");

	fDimState = dis_add_service(stateName, "I", &fState, sizeof(int), 0, 0);
	fDimInfo = dis_add_service(infoName, "C", &fInfo, sizeof(fInfo), 0, 0);
	fDimLogging = dis_add_service(loggingName, "C", &fLogging, sizeof(fLogging), 0, 0);
	fDimConfig = dis_add_service(configName, "C", &fConfig, sizeof(fConfig), 0, 0);

	fSourceID = sourceID;

	dis_start_serving(dimServerName);

	print(dimServerName);

	println(" is starting.");
	centralizedLog(0, "Starting server", 1, 0);

	while(1){
		main_loop();
	}
}

int getRunNumber(){
	return fRunNumber;
}

void setRunNumber(int runNumber) {
	fRunNumber = runNumber;
}

int getState(){
	return fState;
}

void setState(int state) {
	fState = state;
	fNextState = -1;	// Reset the next expected state.
	dis_update_service(fDimState);
}

void moveToExpectedState() {
	//Update the current state to the next expected state only if set (!=-1)
	if(fNextState!=-1) setState(fNextState);
	else setState(fState);
}

void setNextState(int nextState) {
	fNextState = nextState;
}

void publishConfig(){
	char ss[CONFIG_MAX_LENGTH];

	//Generate the config stream (implemented in derived class)
	generateConfig(ss);

	//Put the config stream in the buffer
	strcpy(fConfig, ss);
	dis_update_service(fDimConfig);
}

void waitConfigurationFile(int expectedState) {
	println("... Waiting for configuration file");
	setNextState(expectedState);
}
