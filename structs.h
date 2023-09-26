/*  
structs.h
Teresa Margarida Vargas de Matos ist 1106009
Ficheiro auxilar que manipula paragens e carreiras
*/

#ifndef STRUCTS
#define STRUCTS

#define TRUE 1
#define FALSE 0

struct stop{
    char *nome;
    double latitude, longitude;
}; 
typedef struct stop stop;

struct Pnode{
    struct stop * stop;
    struct Pnode *next, *prev;
};
typedef struct Pnode Pnode;
typedef struct Pnode * Plink;

struct Lnode{
    struct stop * stop;
    struct Lnode *next, *prev;
    double prevcost, prevduration;
    double nextcost, nextduration;
};
typedef struct Lnode Lnode;
typedef struct Lnode * Llink;

struct line{
    char *nome;
    Llink head;
    Llink tail;
    double cost, duration;
    int nPara;
};
typedef struct line line;

struct Cnode{
    struct line *line;
    struct Cnode *next, *prev;
};
typedef struct Cnode Cnode;
typedef struct Cnode *Clink;


int compareStops(stop* stop1, stop* stop2);

Clink lookUpline(Clink head, char *nome);

Plink lookUpstop(Plink head, char *nome);

Clink insertNodeline(Clink head, line * line);

Plink insertNodestop(Plink head, stop * stop);

stop * Createstop(char * nome, double lat, double longi);

line * Createline(char * nome);

Plink removeNodestop( Plink stop, Plink headline);

Clink removeNodeline(Clink line, Clink Headline);

void * MyMalloc(unsigned int size);

Clink popline(Clink head);

Plink popstop(Plink head);

Llink popRoute(Llink head);

line * DeleteRoute(line * line);

void Deleteline(line * line);

void longlat(stop * stop);

int countlines(Clink Headlines);

void resetline(line * line);

#endif