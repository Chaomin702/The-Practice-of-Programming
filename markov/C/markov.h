#ifndef _MARKOV_H_
#define _MARKOV_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum{
  NPREF = 2,
  NHASH = 4093,
  MAXGEN = 10000
};
enum{ MULTIPLIER = 31};
char NOWORD[] = "\n";
typedef struct State State;
typedef struct Suffix Suffix;
struct State{
  char *pref[NPREF];
  Suffix *suf;
  State *next;
};
struct Suffix{
  char *word;
  Suffix *next;
};

void build(char *prefix[NPREF], FILE *f);
void add(char *prefix[NPREF], char *suffix);
void generate(int nwords);
#endif
