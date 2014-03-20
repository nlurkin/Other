#include "dis.h"
#include "decoder.h"
#include "helper.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>
#include "TestNode.h"

int state=0;
int nextState=-1;
char info[STRING_MAX_LENGTH];
char logging[STRING_MAX_LENGTH];
char config[CONFIG_MAX_LENGTH];

int infoIndex=0;

int dimState=0;
int dimInfo=0;
int dimLogging=0;
int dimConfig=0;

int runNumber=0;
double frequency=0.;
int sourceID=0;
char uselessString[STRING_MAX_LENGTH];
int uselessInt=0;
int param=0;

TestNode structNode;

void dimCommand(long* tag, char* cmnd_buffer, int*size){
	print("Command port receiving: ");
	println(cmnd_buffer);

	char args[5][STRING_MAX_LENGTH];

	if(tokenize(args, cmnd_buffer, " ")>0){
		if(strcmp(args[0], "initialize")==0) doInitialize(args);
		if(strcmp(args[0], "startrun")==0) doStartRun(args);
		if(strcmp(args[0], "endrun")==0) doEndRun(args);
		if(strcmp(args[0], "resetstate")==0) doResetState(args);
	}
}

void dimFileContent(long* tag, char* cmnd_buffer, int*size){
	print("FileContent port receiving: ");
	println(cmnd_buffer);

	parseFile(cmnd_buffer, &structNode);

	uselessInt = structNode.uselessInt;
	strcpy(uselessString, structNode.uselessString);
	param = structNode.param;
	frequency = structNode.frequency;
	sourceID = structNode.sourceID;

	println("Finished processing config file... Moving to next state.");
	setState(nextState);
	nextState = -1;
}

void dimRequestConfig(long* tag, char* cmnd_buffer, int*size){
	print("RequestConfig port receiving: ");
	printlni(*cmnd_buffer);

	if(*cmnd_buffer==1){
		sprintf(config, "uselessInt=%i\nparam=%i\nsourceID=%#x\nfrequency=%f\nuselessString=%s\n",
				uselessInt, param, sourceID, frequency, uselessString);

		dis_update_service(dimConfig);
	}
	else{
		println("Unexpected value received from FileContent port.");
		setState(kUNKNOWN);
	}
}

void main_loop(){
	sleep(10);
}

void init_server(char *dimServerName){
	char stateName[STRING_MAX_LENGTH];
	char infoName[STRING_MAX_LENGTH];
	char loggingName[STRING_MAX_LENGTH];
	char configName[CONFIG_MAX_LENGTH];

	char commandName[CONFIG_MAX_LENGTH];
	char fileContentName[CONFIG_MAX_LENGTH];
	char requestConfigName[CONFIG_MAX_LENGTH];

	sprintf(stateName, "%s/%s", dimServerName, "State");
	sprintf(infoName, "%s/%s", dimServerName, "Info");
	sprintf(loggingName, "%s/%s", dimServerName, "Logging");
	sprintf(configName, "%s/%s", dimServerName, "Config");

	sprintf(commandName, "%s/%s", dimServerName, "Command");
	sprintf(fileContentName, "%s/%s", dimServerName, "FileContent");
	sprintf(requestConfigName, "%s/%s", dimServerName, "RequestConfig");

	dimState = dis_add_service(stateName, "I", &state, sizeof(int), 0, 0);
	dimInfo = dis_add_service(infoName, "C", &info, sizeof(info), 0, 0);
	dimLogging = dis_add_service(loggingName, "C", &logging, sizeof(logging), 0, 0);
	dimConfig = dis_add_service(configName, "C", &config, sizeof(config), 0, 0);

	dis_add_cmnd(commandName, "C", dimCommand, 0);
	dis_add_cmnd(fileContentName, "C", dimFileContent, 0);
	dis_add_cmnd(requestConfigName, "I", dimRequestConfig, 0);
}

int main(int argc, char** argv){
	char dimServerName[STRING_MAX_LENGTH] = "dimExample";

	init_server(dimServerName);

	dis_start_serving(dimServerName);
	print(dimServerName);
	println(" is starting.");
	centralizedLog(0, "Starting server", 1, 0);

	while(1){
		main_loop();
	}
}
