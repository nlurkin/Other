#ifndef HELPER_H
#define HELPER_H

#define STRING_MAX_LENGTH 500
#define CONFIG_MAX_LENGTH 1000

int tokenize(char tok[][STRING_MAX_LENGTH], char* s, const char *delim);

void print(const char * s);
void printi(int s);
void println(const char *s);
void printlni(int s);
void centralizedLog(int severity, char* text, int priority, int errCode);

//###################
// To be found in server.c
extern int fDimInfo;
extern int fDimLogging;

extern char fInfo[STRING_MAX_LENGTH];
extern char fLogging[STRING_MAX_LENGTH];

extern int fInfoIndex;
extern int fSourceID;
//###################

#endif
