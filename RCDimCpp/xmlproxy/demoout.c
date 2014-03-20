#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TestNode.h"
#include "xmlconfig_TestNode.h" /* include file prepared by the xmlpp preprocessor */

int main(){	
	
    TestNode node;
    
    
    node.uselessInt = 5;
    node.param = 10;
    node.sourceID = 0x11;
    node.frequency = 1000.2;
    node.uselessInt = "toto";

    node.version=TestNode_VERSION;
    
    OUTXMLFILE_TestNode(node, "ColdConfig.xml");  //macro defined in xmlconfig_Node.h
	
    return 0;
}
