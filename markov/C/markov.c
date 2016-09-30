#include "markov.h"
State *statetab[NHASH];

unsigned int hash(char *s[NPREF]){
  unsigned int h;
  unsigned char *p;
  int i;
  h = 0;
  for(i=0;i<NPREF;++i)
    for(p = (unsigned char *)s[i];*p!='\0';++p)
      h = MULTIPLIER * h + *p;
  return h%NHASH;
}

State* lookup(char *prefix[NPREF], int create){
  int h,i;
  State *p;

  h = hash(prefix);
  for(p = statetab[h];p!=NULL;p=p->next){
    for(i=0;i<NPREF;++i){
      if(strcmp(prefix[i],p->pref[i])!=0)
        break;
    }
    if(i == NPREF)
      return p;
  }
  if(create){
    p = (State *)malloc(sizeof(State));
    if(p==NULL)
      exit(1);
    for(i=0;i<NPREF;++i)
      p->pref[i] = prefix[i];
    p->suf = NULL;
    p->next = statetab[h];
    statetab[h] = p;
  }
  return p;
}

void build(char *prefix[NPREF], FILE *f){
  char buf[100], fmt[10];
  sprintf(fmt, "%%%ds", sizeof(buf-1));
  while(fscanf(f,fmt,buf)!=EOF)
    add(prefix,strdup(buf));
}

void addsuffix(State *sp, char *suffix){
  Suffix *p = (Suffix *)malloc(sizeof(Suffix));
  if(p==NULL)
    exit(1);
  p->word = suffix;
  p->next = sp->suf;
  sp->suf = p;
}

void add(char *prefix[NPREF], char *suffix){
  State *p = lookup(prefix, 1);
  addsuffix(p,suffix);

  memmove(prefix,prefix+1,(NPREF-1)*sizeof(prefix[0]));
  prefix[NPREF-1] = suffix;
}



void generate(int nword){
  char *pref[NPREF], *w;
  Suffix *suf;
  State *p;
  int i, nmatch;
  for(i=0;i<NPREF;++i)
    pref[i] = NOWORD;
  for(i=0;i<nword;++i){
    p = lookup(pref,0);
    nmatch = 0;
    for(suf = p->suf;suf!=NULL;suf = suf->next){
      if( (rand()% ++nmatch) == 0)
        w = suf->word;
    }
    if(strcmp(w,NOWORD) == 0)
      break;
    printf("%s ",w);
    memmove(pref,pref+1,(NPREF-1)*sizeof(pref[0]));
    pref[NPREF-1] = w;
  }
}

int main(void){
  int i, nwords = MAXGEN;
  char *prefix[NPREF];
  for(i=0;i<NPREF;++i)
    prefix[i] = NOWORD;
  build(prefix,stdin);
  add(prefix,NOWORD);
  generate(nwords);
  return 0;
}

