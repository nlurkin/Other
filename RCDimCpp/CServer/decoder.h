#include "helper.h"
#include "TestNode.h"

void parseFile(char* content, TestNode *s);
int tokenize(char tok[][STRING_MAX_LENGTH], char* s, const char *delim);
void decodeLine(char* line);
