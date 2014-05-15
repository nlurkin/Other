#include "dis.h"
#include "decoder.h"
#include "server.h"
#include "commands.h"
#include <stdio.h>
#include "TestNode.h"
#include <string.h>

double frequency=0.;
int sourceID=0;
char uselessString[STRING_MAX_LENGTH];
int uselessInt=0;
int param=0;

struct configStruct_t *fConfigStruct;

char dimServerName[STRING_MAX_LENGTH] = "dimExample";

void doEndRun(char tok[5][STRING_MAX_LENGTH]){
	if(fState==kREADY){
		print("Stopping current run (");
		printi(fRunNumber);
		println(")");
		setState(kINITIALIZED);
	}
	else{
		println("Device is not in READY state. Cannot stop a run.");
		setState(kWRONGSTATE);
	}
}

void doResetState(char tok[5][STRING_MAX_LENGTH]) {
	println("Reset requested");
	setState(kIDLE);
}

void generateConfig(char* configString){
	centralizedLog(0, "Toto", 1, 0);
	//Generate the current configuration stream using the same format as the input file.
	sprintf(configString, "Configuration/Report/SubSystem/%s/report.xml", dimServerName);

	fConfigStruct->t->frequency = frequency;
	fConfigStruct->t->param = param;
	fConfigStruct->t->sourceID = sourceID;
	fConfigStruct->t->uselessInt = uselessInt;
	strcpy(fConfigStruct->t->uselessString, uselessString);

	writeFile(configString, fConfigStruct);
}

int selectCommand(char commandName[STRING_MAX_LENGTH], char tok[5][STRING_MAX_LENGTH]){
	if(strcmp(commandName, "initialize")==0) doStdInitialize(tok);
	else if(strcmp(commandName, "startrun")==0) doStdStartRun(tok);
	else if(strcmp(commandName, "endrun")==0) doEndRun(tok);
	else if(strcmp(commandName, "resetstate")==0) doResetState(tok);
	else return -1;

	return 0;
}

int applyConfiguration(){
	param = fConfigStruct->t->param;
	sourceID = fConfigStruct->t->sourceID;
	uselessInt = fConfigStruct->t->uselessInt;
	frequency = fConfigStruct->t->frequency;
	strcpy(uselessString,fConfigStruct->t->uselessString);

	return 0;
}


int main_loop(){
	sleep(10);
	return 0;
}


int main(int argc, char** argv){
	if(argc>1) strcpy(dimServerName, argv[1]);

	fConfigStruct = (configStruct*)malloc(sizeof(configStruct));
	fConfigStruct->t = (TestNode*)malloc(sizeof(TestNode));
	initCommands(dimServerName, NULL, NULL, NULL);
	startServer(dimServerName, 0x13);
}
