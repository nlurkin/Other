/*
 * server.h
 *
 *  Created on: Mar 21, 2014
 *      Author: nlurkin
 */

#ifndef SERVER_H_
#define SERVER_H_

enum FSMState {kIDLE=0, kINITIALIZED=1, kREADY=2};
enum ErrorState {kHARDWAREFAILURE=-10, kCONFIGERROR=-11, kWRONGSTATE=-12, kUNKNOWN=-20};

void initCommands(char* dimServerName, void (*commandRoutine)(void*,void*,int*), void (*fileContentRoutine)(void*,void*,int*), void (*requestConfigRoutine)(void*,void*,int*));
void startServer(char* dimServerName, int sourceID);

int getRunNumber();
void setRunNumber(int runNumber);

int getState();
void setState(int state);

void moveToExpectedState();
void setNextState(int nextState);
void waitConfigurationFile(int expectedState);

void publishConfig();

//###########################
//To be implemented by user
typedef struct configStruct_t configStruct;
extern configStruct *fConfigStruct;
extern void generateConfig(char* configString);
extern int main_loop();
//###########################

#endif /* SERVER_H_ */
