/*
 * helper.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */
#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "dis.h"

void print(const char * s){
	printf("%s",s);
	strcpy(info+infoIndex, s);
	infoIndex+=strlen(s);
}
void printi(int s){
	char dest[STRING_MAX_LENGTH];
	sprintf(dest,"%i", s);
	print(dest);
}
void println(const char *s){
	printf("%s\n",s);
	strcpy(info+infoIndex, s);
	info[infoIndex+strlen(s)] = '\n';
	info[infoIndex+strlen(s)+1] = '\0';
	infoIndex = 0;
	dis_update_service(dimInfo);
}
void printlni(int s){
	char dest[STRING_MAX_LENGTH];
	sprintf(dest,"%i", s);
	println(dest);
}

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
	sprintf(numValue, "%#x", sourceID);
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

	strcpy(logging,str);
	dis_update_service(dimLogging);
}
