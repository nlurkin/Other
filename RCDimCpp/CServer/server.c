/*
 * server.c
 *
 *  Created on: Mar 21, 2014
 *      Author: nlurkin
 */

/**
 * @file server.c
 * Contains the functions necessary to initialize and run the DIM server
 * with the behavior described in the Note NA62-14-02.
 */

#include "server.h"
#include "helper.h"
#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include "dis.h"
#include <string.h>

int fDimState=0;	/*!< Identifier of dimServerName/State service */
int fDimInfo=0;		/*!< Identifier of dimServerName/Info service */
int fDimLogging=0;	/*!< Identifier of dimServerName/Logging service */
int fDimConfig=0;	/*!< Identifier of dimServerName/Config service */

int fRunNumber=0;	/*!< Current Run Number */

int fState=0;						/*!< Current state of the device */
int fNextState=-1;					/*!< Expected state of the device after receiving and applying configuration file*/
char fInfo[STRING_MAX_LENGTH];		/*!< Buffer for dimServerName/Info */
char fLogging[STRING_MAX_LENGTH];	/*!< Buffer for dimServerName/Logging */
char fConfig[CONFIG_MAX_LENGTH];	/*!< Buffer for dimServerName/Config */

int fInfoIndex=0;	/*!< Current index of the fInfo buffer */
int fSourceID=0;	/*!< Source ID of the device */

/**
 * Initialize the dim commands.
 *
 * The user is expected to pass a function pointer to a function handling respectively the
 * dimServerName/Command, dimServerName/FilContent and dimServerName/RequestConfig dim commands.
 * For each command where the standard implementation should be used, pass a NULL pointer.
 * @param dimServerName Name of the DIM server
 * @param commandRoutine Function pointer to the function handling dimServerName/Command
 * @param fileContentRoutine Function pointer to the function handling dimServerName/FileContent
 * @param requestConfigRoutine Function pointer to the function handling dimServerName/RequestConfig
 */
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

/**
 * Initialize and start the DIM server. Starts a loop calling
 * main_loop() function (to be implemented by user). The loop
 * exits when main_loop() returns 1.
 *
 * 4 services are created : dimServerName/State, dimServerName/Info, dimServerName/Logging and dimServerName/Config.
 * @param dimServer Name Name of the DIM server
 * @param sourceID Source ID of the device
 */
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

	while(main_loop()!=1){
	}
}

/**
 *
 * @return The current Run Number
 */
int getRunNumber(){
	return fRunNumber;
}

/**
 * Set the current Run Number
 * @param runNumber current Run Number
 */
void setRunNumber(int runNumber) {
	fRunNumber = runNumber;
}

/**
 *
 * @return Current device state
 */
int getState(){
	return fState;
}

/**
 * Set the current device state.
 * Reset the next expected state.
 * @param state State to set
 */
void setState(int state) {
	fState = state;
	fNextState = -1;	// Reset the next expected state.
	dis_update_service(fDimState);
}

/**
 * Set the current device state to the next expected state
 * (if set, else stay in the current state).
 */
void moveToExpectedState() {
	//Update the current state to the next expected state only if set (!=-1)
	if(fNextState!=-1) setState(fNextState);
	else setState(fState);
}

/**
 * Set the next expected state.
 * @param nextState next expected state
 */
void setNextState(int nextState) {
	fNextState = nextState;
}

/**
 * Publish the current configuration to dimServerName/Config.
 * The current configuration is requested to generateConfig (to be implemented by user)
 */
void publishConfig(){
	char ss[CONFIG_MAX_LENGTH];

	//Generate the config stream (implemented in derived class)
	generateConfig(ss);

	//Put the config stream in the buffer
	strcpy(fConfig, ss);
	dis_update_service(fDimConfig);
}

/**
 * Wait for a configuration file and set the next expected state when
 * the file is correctly decoded and applied.
 * @param expectedState next expected state after successfully applying the configuration
 */
void waitConfigurationFile(int expectedState) {
	println("... Waiting for configuration file");
	setNextState(expectedState);
}

