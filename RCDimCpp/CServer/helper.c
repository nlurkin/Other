/*
 * helper.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */
#include <stdio.h>
#include <string.h>
#include "helper.h"

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
	dis_update_service_(dimInfo);
}
void printlni(int s){
	char dest[STRING_MAX_LENGTH];
	sprintf(dest,"%i", s);
	println(dest);
}
