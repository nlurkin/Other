#ifndef COMMANDS_H
#define COMMANDS_H

void doStdInitialize(char tok[5][STRING_MAX_LENGTH]);
void doStdStartRun(char tok[5][STRING_MAX_LENGTH]);
void doStdEndRun(char tok[5][STRING_MAX_LENGTH]);
void doStdResetState(char tok[5][STRING_MAX_LENGTH]);

void stdDimCommand(long* tag, char* cmnd_buffer, int*size);
void stdDimFileContent(long* tag, char* cmnd_buffer, int*size);
void stdDimRequestConfig(long* tag, char* cmnd_buffer, int*size);

int stdSelectCommand(char commandName[STRING_MAX_LENGTH], char tok[5][STRING_MAX_LENGTH]);

void enableFileContent();
void disableFileContent();

//#########################
//To be found in server.c
extern void publishConfig();

extern int fState;
extern int fNextState;
extern int fDimConfig;
extern int fRunNumber;
//#########################

//#########################
//To be implemented by user
extern int selectCommand(char commandName[STRING_MAX_LENGTH], char tok[5][STRING_MAX_LENGTH]);
extern int parseFile(char* content, struct configStruct_t *s);
extern int applyConfiguration();
//#########################

#endif
