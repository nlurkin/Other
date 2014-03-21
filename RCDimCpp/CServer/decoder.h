#include "helper.h"
#include "TestNode.h"

struct configStruct_t {
	TestNode *t;
};
int parseFile(char* content, struct configStruct_t *s);
//void decodeLine(char* line);
