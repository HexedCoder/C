#include "human.h"

#define MAX_LEN 32

struct human_t {
  char *fname;
  char *lname;
};

human_t *human_init(const char *fname, const char *lname) {
  human_t *human = NULL;
  if (!fname || !lname) {
    fprintf(stderr, "[ERR!] Invalid input\n");
    goto HUMAN_INIT;
  }

  if (MAX_LEN < strnlen(fname, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", fname, MAX_LEN);
    goto HUMAN_INIT;
  }

  if (MAX_LEN < strnlen(lname, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", lname, MAX_LEN);
    goto HUMAN_INIT;
  }

  human = calloc(1, sizeof(*human));
  if (!human) {
    fprintf(stderr, "[ERR!] Cannot allocate\n");
    goto HUMAN_INIT;
  }

  // No need for strndup since we already bounds checked
  //
  human->fname = strdup(fname);
  if (!human->fname) {
    fprintf(stderr, "[ERR!] Cannot allocate for fname\n");
    free(human);
    goto HUMAN_INIT;
  }

  human->lname = strdup(lname);
  if (!human->lname) {
    fprintf(stderr, "[ERR!] Cannot allocate for lname\n");
    free(human->fname);
    free(human);
    goto HUMAN_INIT;
  }

HUMAN_INIT:
  return human;
}

void human_print(human_t *human) {
  if (!human) {
    return;
  }

  printf("%s, %s\n", human->lname, human->fname);
}

int human_departs(human_t **human) {
  int ret = 0;
  if (!human || !*human) {
    fprintf(stderr, "[ERR!] Cannot dereference\n");
    goto HUMAN_DEPARTED;
  }

  free((*human)->fname);
  free((*human)->lname);
  free(*human);
  *human = NULL;
  ret = 1;

HUMAN_DEPARTED:
  return ret;
}

int human_rename(human_t *human, const char *fname, const char *lname) {
  int ret = 0;
  if (!human || !fname || !lname) {
    fprintf(stderr, "[ERR!] NULL pointer given\n");
    goto HUMAN_RENAME;
  }

  if (MAX_LEN < strnlen(fname, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", fname, MAX_LEN);
    goto HUMAN_RENAME;
  }

  if (MAX_LEN < strnlen(lname, MAX_LEN + 1)) {
    fprintf(stderr, "[ERR!] %s exceeds %d chars\n", lname, MAX_LEN);
    goto HUMAN_RENAME;
  }

  free(human->fname);
  free(human->lname);

  char *ftemp = strdup(fname);
  human->fname = strdup(fname);
  if (!ftemp) {
    fprintf(stderr, "[ERR!] Cannot allocate new fname\n");
    goto HUMAN_RENAME;
  }

  char *ltemp = strdup(lname);
  if (!ltemp) {
    fprintf(stderr, "[ERR!] Cannot allocate new lname\n");
    free(ftemp);
    goto HUMAN_RENAME;
  }

  human->fname = ftemp;
  human->lname = ltemp;
  ret = 1;

HUMAN_RENAME:
  return ret;
}
