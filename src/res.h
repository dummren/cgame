#ifndef __CG__RES_H__
#define __CG__RES_H__

#define CG_RES_MALLOC(path) malloc(sizeof(unsigned char) * cgResLen(path))

void cgResInit();

int cgResLen(const char*);
void cgResRead(const char*, unsigned char*);

#endif // __CG__RES_H__
