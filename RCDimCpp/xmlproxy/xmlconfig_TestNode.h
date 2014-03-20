/* This file is always created automatically by running xmlcpp preprocessor
#Do not edit it ! */
#ifndef __xmlconfig_TestNode_h__
#define __xmlconfig_TestNode_h__
#define OUTXML_TestNode(var) outxml_TestNode(&var,#var)
#define INXML_TestNode(var) inxml_TestNode(&var,#var)
#define OUTXMLFILE_TestNode(var,filename) outxmlfile_TestNode(&var,#var,filename)
#define INXMLFILE_TestNode(var,filename) inxmlfile_TestNode(&var,#var, filename)
#ifdef __cplusplus
extern "C" {
#endif
void outxml_TestNode(void* top_struct, char* varname);
void inxml_TestNode(void* top_struct, char* varname);
void outxmlfile_TestNode(void* top_struct, char* varname, char* filename);
void inxmlfile_TestNode(void* top_struct, char* varname, char* filename);
#ifdef __cplusplus
}
#endif
#endif /* __xmlconfig_TestNode_h__ */
