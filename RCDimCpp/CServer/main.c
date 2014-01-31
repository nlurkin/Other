#include "dis.h"
#include "helper.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>

int state=0;
int waiting=0;
char info[STRING_MAX_LENGTH];
char logging[STRING_MAX_LENGTH];
char config[CONFIG_MAX_LENGTH];

int infoIndex=0;

int dimState=0;
int dimInfo=0;
int dimLogging=0;
int dimWaiting=0;
int dimConfig=0;

int runNumber=0;
double frequency=0.;
int sourceID=0;
char uselessString[STRING_MAX_LENGTH];
int uselessInt=0;
int param=0;

void dimCommand(long* tag, char* cmnd_buffer, int*size){
	print("Command port receiving: ");
	println(cmnd_buffer);

	char *cmnd = strtok(cmnd_buffer, " ");
	char args[5][STRING_MAX_LENGTH];
	char *t;
	int i=0;
	if(cmnd!= NULL){
		t = strtok(NULL, " ");
		while(t != NULL){
			strcpy(args[i], t);
			t = strtok(NULL, " ");
		}
	}

	if(strcmp(cmnd, "initialize")==0) doInitialize(args);
	if(strcmp(cmnd, "startrun")==0) doStartRun(args);
	if(strcmp(cmnd, "endrun")==0) doEndRun(args);
	if(strcmp(cmnd, "resetstate")==0) doResetState(args);
}

void dimFileContent(long* tag, char* cmnd_buffer, int*size){
	printf("Waiting : %i\n", waiting);
	if(waiting==1){
		print("FileContent port receiving: ");
		println(cmnd_buffer);

		/*decoder.parseFile(getString());
		//Applying parameters
		setSourceId(decoder.param3);
		setFrequency(decoder.param4);
		setUselessInt(decoder.param1);
		setUselessString(decoder.param5);
		setParam(decoder.param2);
		*/

		println("Finished processing config file... Waiting for next one.");
		setWaiting(1);
	}
	else{
		setState(state);
	}
}

void dimEndTransfer(long* tag, char* cmnd_buffer, int*size){
	if(waiting==1){
		print("EndTransfer port receiving: ");
		printlni(*cmnd_buffer);

		if(*cmnd_buffer==1){
			sprintf(config, "uselessInt=%i\nparam=%i\nsourceID=%#x\nfrequency=%f\nuselessString=%s\n",
					uselessInt, param, sourceID, frequency, uselessString);

			setWaiting(3);
			dis_update_service(dimConfig);
			setState(kINITIALIZED);
		}
		else{
			println("Unexpected value received from FileContent port.");
			setState(kUNKNOWN);
		}
	}
}

void main_loop(){
	sleep(10);
}

void init_server(char *dimServerName){
	char stateName[STRING_MAX_LENGTH];
	char infoName[STRING_MAX_LENGTH];
	char loggingName[STRING_MAX_LENGTH];
	char waitingName[STRING_MAX_LENGTH];
	char configName[CONFIG_MAX_LENGTH];

	char commandName[CONFIG_MAX_LENGTH];
	char fileContentName[CONFIG_MAX_LENGTH];
	char endTransferName[CONFIG_MAX_LENGTH];

	sprintf(stateName, "%s/%s", dimServerName, "State");
	sprintf(infoName, "%s/%s", dimServerName, "Info");
	sprintf(loggingName, "%s/%s", dimServerName, "Logging");
	sprintf(waitingName, "%s/%s", dimServerName, "Waiting");
	sprintf(configName, "%s/%s", dimServerName, "Config");

	sprintf(commandName, "%s/%s", dimServerName, "Command");
	sprintf(fileContentName, "%s/%s", dimServerName, "FileContent");
	sprintf(endTransferName, "%s/%s", dimServerName, "EndTransfer");

	dimState = dis_add_service(stateName, "I", &state, sizeof(int), 0, 0);
	dimInfo = dis_add_service(infoName, "C", &info, sizeof(info), 0, 0);
	dimLogging = dis_add_service(loggingName, "C", &logging, sizeof(logging), 0, 0);
	dimWaiting = dis_add_service(waitingName, "I", &waiting, sizeof(int), 0, 0);
	dimConfig = dis_add_service(configName, "C", &config, sizeof(config), 0, 0);

	dis_add_cmnd(commandName, "C", dimCommand, 0);
	dis_add_cmnd(fileContentName, "C", dimFileContent, 0);
	dis_add_cmnd(endTransferName, "I", dimEndTransfer, 0);
}

int main(int argc, char** argv){
	char dimServerName[STRING_MAX_LENGTH] = "dimExample";

	init_server(dimServerName);

	dis_start_serving(dimServerName);
	print(dimServerName);
	println(" is starting.");

	while(1){
		main_loop();
	}
}
