/*
 * decoder.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */

#include "decoder.h"
#include <stdlib.h>
#include <string.h>
#include "xmlconfig_TestNode.h"
#include <stdio.h>

int parseFile(char *content, struct configStruct_t *s){
	/*char lines[10][STRING_MAX_LENGTH];

	int linesNb = tokenize(lines, content, "\n");
	int i;
	for(i=0; i<linesNb; i++){
		decodeLine(lines[i]);
	}*/

	inxmlfile_TestNode(s->t,"toto", content);
	return 0;
}

int writeFile(char *content, struct configStruct_t *s){
	/*char lines[10][STRING_MAX_LENGTH];

	int linesNb = tokenize(lines, content, "\n");
	int i;
	for(i=0; i<linesNb; i++){
		decodeLine(lines[i]);
	}*/

	outxmlfile_TestNode((void*)s->t, "toto", content);
	return 0;
}

/*void decodeLine(char *line){
	char tok[2][STRING_MAX_LENGTH];

	if(tokenize(tok, line, "=")!=2) return;
	if(strcmp(tok[0],"uselessInt")==0) uselessInt = atoi(tok[1]);
	if(strcmp(tok[0],"param")==0) param = atoi(tok[1]);
	if(strcmp(tok[0],"sourceID")==0) sourceID = strtol(tok[1], NULL, 16);
	if(strcmp(tok[0],"frequency")==0) frequency = atof(tok[1]);
	if(strcmp(tok[0],"uselessString")==0) strcpy(uselessString,tok[1]);
}*/
