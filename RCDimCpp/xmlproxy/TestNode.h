/*
 * TestNode.h
 *
 *  Created on: Mar 20, 2014
 *      Author: nlurkin
 */

#ifndef TESTNODE_H_
#define TESTNODE_H_

#include "xmlstring.h"

#define TestNode_VERSION 100


typedef struct __TestNode
{

	int version; /*jj*/

	int uselessInt;
	int param;
	int sourceID;
	double frequency;
	xmlchar uselessString[XMLSTRING];
} TestNode;

#endif /* TESTNODE_H_ */
