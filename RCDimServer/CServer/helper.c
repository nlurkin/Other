/*
 * helper.c
 *
 *  Created on: 31 Jan 2014
 *      Author: Nicolas Lurkin
 */
#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "dis.h"

/**
 * @file helper.c
 * Contains some useful functions.
 */

/**
 * Output to cout without endline and without flushing
 * and append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void print(const char * s){
	printf("%s",s);
	strcpy(fInfo+fInfoIndex, s);
	fInfoIndex+=strlen(s);
}
/**
 * Output to cout without endline and without flushing
 * and append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void printi(int s){
	char dest[STRING_MAX_LENGTH];
	sprintf(dest,"%i", s);
	print(dest);
}
/**
 * Output to cout with endline (and flush the stream)
 * and append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void println(const char *s){
	printf("%s\n",s);
	strcpy(fInfo+fInfoIndex, s);
	fInfo[fInfoIndex+strlen(s)] = '\n';
	fInfo[fInfoIndex+strlen(s)+1] = '\0';
	fInfoIndex = 0;
	if(fIsStarted==1) dis_update_service(fDimInfo);
}
/**
 * Output to cout with endline (and flush the stream)
 * and append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void printlni(int s){
	char dest[STRING_MAX_LENGTH];
	sprintf(dest,"%i", s);
	println(dest);
}

/**
 * Output an entry in the centralized logging display according to the standard format.
 *
 * @param severity Severity of the entry
 * @param text Text message of the entry
 * @param priority Priority of the entry (for display)
 * @param errCode Error code of the entry (default=0
 */
void centralizedLog(int severity, char* text, int priority, int errCode)
{
	char str[500];
	char numValue[10];

	time_t mtime = time(NULL);
	char *time= ctime(&mtime);

	strcpy(str, "ErrTime:");
	strcat(str, time);
	strcat(str, "<[|]>");
	strcat(str, "SysName:");
	sprintf(numValue, "%#x", fSourceID);
	strcat(str, numValue);
	strcat(str, "<[|]>");
	strcat(str, "Username:n/a<[|]>");
	strcat(str, "Manager:n/a<[|]>");
	strcat(str, "DpId:n/a<[|]>");
	strcat(str, "ErrPrio:");
	sprintf(numValue, "%i", priority);
	strcat(str, numValue);
	strcat(str, "<[|]>");
	strcat(str, "ErrType:0<[|]>");
	strcat(str, "ErrCode:");
	sprintf(numValue, "%i", errCode);
	strcat(str, numValue);
	strcat(str, "<[|]>");
	strcat(str, "ErrText:");
	strcat(str, text);
	strcat(str, "<[|]>");

	strcpy(fLogging,str);
	if(fIsStarted==1) dis_update_service(fDimLogging);
}

/**
 * Tokenize a string according to the specified delimiter
 * @param tok: array of char to place the tokens
 * @param s: string to tokenize
 * @param delim: delimiter
 * @return
 */
int tokenize(char tok[][STRING_MAX_LENGTH], char*s, const char *delim){
	char *t;
	int i=0;
	t = strtok(s, delim);
	while(t != NULL){
		strcpy(tok[i], t);
		t = strtok(NULL, delim);
		i++;
	}
	return i;
}
