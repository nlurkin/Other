#ifndef HELPER_H
#define HELPER_H

#define STRING_MAX_LENGTH 100
#define CONFIG_MAX_LENGTH 1000

extern int state;
extern int nextState;
extern char info[STRING_MAX_LENGTH];
extern char logging[STRING_MAX_LENGTH];
extern char config[CONFIG_MAX_LENGTH];

extern int infoIndex;

extern int dimState;
extern int dimInfo;
extern int dimLogging;
//extern int dimWaiting;
extern int dimConfig;

extern int runNumber;
extern double frequency;
extern int sourceID;
extern char uselessString[STRING_MAX_LENGTH];
extern int uselessInt;
extern int param;

enum FSMState {kIDLE=0, kINITIALIZED=1, kREADY=2};
enum ErrorState {kHARDWAREFAILURE=-10, kWRONGCONFIG=-11, kWRONGSTATE=-12, kUNKNOWN=-20};

void print(const char * s);
void printi(int s);
void println(const char *s);
void printlni(int s);

#endif
