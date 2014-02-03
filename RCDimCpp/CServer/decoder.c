/*
 * decoder.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */

#include "decoder.h"
#include <stdlib.h>
#include <string.h>

void parseFile(char *content){
	char lines[10][STRING_MAX_LENGTH];

	int linesNb = tokenize(lines, content, "\n");
	int i;
	for(i=0; i<linesNb; i++){
		decodeLine(lines[i]);
	}
}

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

void decodeLine(char *line){
	char tok[2][STRING_MAX_LENGTH];

	if(tokenize(tok, line, "=")!=2) return;
	if(strcmp(tok[0],"uselessInt")==0) uselessInt = atoi(tok[1]);
	if(strcmp(tok[0],"param")==0) param = atoi(tok[1]);
	if(strcmp(tok[0],"sourceID")==0) sourceID = strtol(tok[1], NULL, 16);
	if(strcmp(tok[0],"frequency")==0) frequency = atof(tok[1]);
	if(strcmp(tok[0],"uselessString")==0) strcpy(uselessString,tok[1]);
}
