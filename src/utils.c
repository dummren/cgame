#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

unsigned int cgUtilsStrCount(const char *str, const char *what) {
  unsigned int count = 0;

  for (unsigned int i = 0; i <= strlen(str) - strlen(what); i++) {
    char strseq[strlen(what) + 1];

    for (unsigned int ii = 0; ii < strlen(what); ii++)
      strseq[ii] = str[i + ii];

    strseq[strlen(what)] = 0;

    if (strcmp(strseq, what) == 0)
      count++;
  }

  return count;
}

int cgUtilsStrFind(const char *str, const char *what,
                   const unsigned int start) {
  for (unsigned int i = start; i <= strlen(str) - strlen(what); i++) {
    char strseq[strlen(what) + 1];
    for (unsigned int ii = 0; ii < strlen(what); ii++)
      strseq[ii] = str[i + ii];

    strseq[strlen(what)] = 0;

    if (strcmp(strseq, what) == 0)
      return i;
  }

  return -1;
}

char *cgUtilsStrSub(const char *str,
                    const unsigned int start,
                    const unsigned int len) {
  char *out = malloc(len + 1);

  for (unsigned int i = 0; i < len; i++)
    out[i] = str[start + i];

  out[len] = 0;

  return out;
}

void cgUtilsStrSplit(const char *str, const char *sep, char **outLines) {
  size_t len = cgUtilsStrCount(str, "\n\0");
  size_t pos = 0;

  for (size_t i = 0; i < len; i++) {
    size_t start = pos;
    size_t end = cgUtilsStrFind(str, "\n", pos);
    size_t len = end - start;

    outLines[i] = cgUtilsStrSub(str, start, len);
    pos = end + 1;
  }
}

bool cgUtilsStrStartsWith(const char *str, const char *what) {
  for (size_t i = 0; i < strlen(what); i++)
    if (str[i] != what[i])
      return false;

  return true;
}
