/*
 * commands.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */

#include "helper.h"
#include "dis.h"
#include <stdlib.h>

void setState(int i){
	state = i;
	dis_update_service_(dimState);
}

void setRunNumber(int i){
	runNumber = i;
}

void doInitialize(char tok[5][STRING_MAX_LENGTH]){
	if(state!=kREADY){
		println("Initializing... Waiting for configuration file");
		nextState = kINITIALIZED;
	}
}
void doStartRun(char tok[5][STRING_MAX_LENGTH]){
	if(state==kINITIALIZED){
		print("Starting run number ");
		print(tok[1]);
		println("... Waiting for configuration file.");
		setRunNumber(atoi(tok[1]));
		nextState = kREADY;
	}
	else{
		println("Device is not in INITIALIZED state. Cannot start a run.");
		setState(kWRONGSTATE);
	}
}
void doEndRun(char tok[5][STRING_MAX_LENGTH]){
	if(state==kREADY){
		print("Stopping current run (");
		printi(runNumber);
		println(")");
		setState(kINITIALIZED);
	}
	else{
		println("Device is not in READY state. Cannot stop a run.");
		setState(kWRONGSTATE);
	}
}
void doResetState(char tok[5][STRING_MAX_LENGTH]){
	println("Reset requested");
	setState(kIDLE);
}
