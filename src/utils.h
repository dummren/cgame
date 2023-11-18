#ifndef __CG__UTILS_H__
#define __CG__UTILS_H__

#include <stddef.h>
#include <stdbool.h>

unsigned int cgUtilsStrCount(const char*, const char*);
int cgUtilsStrFind(const char*, const char*, const unsigned int);
char *cgUtilsStrSub(const char*, const unsigned int, const unsigned int);
void cgUtilsStrSplit(const char*, const char*, char**);
bool cgUtilsStrStartsWith(const char*, const char*);

#endif // __CG__UTILS_H__
