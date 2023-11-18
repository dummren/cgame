#include "res.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "data.dat.h"

static unsigned char *f_data;
static int f_dataLen;

void cgResInit() {
  f_data = tmp_data_data_dat;
  f_dataLen = tmp_data_data_dat_len;
}

static void f_read(const char *asset, int *outLen, unsigned char *outData) {
  int pos = 0;

  while (pos < f_dataLen) {
    char path[1024];
    char lenStr[1024];

    memset(path, 0, sizeof(path));
    memset(lenStr, 0, sizeof(lenStr));

    while (f_data[pos] != '\n')
      sprintf(path, "%s%c", path, f_data[pos++]);
    pos++;

    while (f_data[pos] != '\n')
      sprintf(lenStr, "%s%c", lenStr, f_data[pos++]);
    pos++;

    int len = atoi(lenStr);

    if (strcmp(path, asset) == 0) {
      if (outLen)
        *outLen = len;

      if (outData)
        for (int i = 0; i < len; i++)
          outData[i] = f_data[pos + i];

      return;
    } else
      pos += len + 1;
  }

  if (outLen)
    *outLen = -1;
}

int cgResLen(const char *path) {
  int len;
  f_read(path, &len, NULL);

  return len;
}

void cgResRead(const char *path, unsigned char *out) {
  f_read(path, NULL, out);
}
