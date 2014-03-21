/*
 * server.h
 *
 *  Created on: Mar 21, 2014
 *      Author: nlurkin
 */

#ifndef SERVER_H_
#define SERVER_H_

typedef struct configStruct_t configStruct;
extern void generateConfig(char* configString);
extern configStruct *fConfigStruct;
extern void main_loop();

void initCommands(char* dimServerName, void (*commandRoutine)(void*,void*,int*), void (*fileContentRoutine)(void*,void*,int*), void (*requestConfigRoutine)(void*,void*,int*));

void startServer(char* dimServerName, int sourceID);

int getRunNumber();
void setRunNumber(int runNumber);

int getState();
void setState(int state);

void moveToExpectedState();
void setNextState(int nextState);

void publishConfig();

void waitConfigurationFile(int expectedState);



#endif /* SERVER_H_ */
