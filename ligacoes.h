/*  
ligacoes.h
Teresa Margarida Vargas de Matos ist 1106009
Ficheiro header auxiliar que manipula ligações
*/

#ifndef LIGACOES
#define LIGACOES
#include "structs.h"

#define TRUE 1
#define FALSE 0 
#define ORIGEM 2
#define DESTINO 3
#define CICLO 4

int connectionType(line * line, stop * origem, stop * destino);

line * addCostDur(line * line, float cost, float duration);

line* changeHead(line * line, stop *origem, double custo, double duracao);

line* changeTail(line * line, stop *destino, double custo, double duracao);

line* addcounterC(line * line);

stop* addcounterP(stop * stop) ;

void listlines(line * line);

void listRoute(line * line);

void listRouteInv(line * line);

void listStops(stop * stop, Clink Headlines);

int getNlines(stop * stop, Clink  Headlines);

void UpdateLine(line * line, stop * origem, stop * destino, double custo, 
double duracao, int mudanca);

void swap(line ** vec, int i, int j);

void selectionsort(line **vec, int n);

int CountStops(line * line);

int nStopsinLine(line * line, stop * stop, int *ntotal);

void removeStopsinLine(line * line, stop * stop);

void removeStopinRoute(line * line, stop * stop);

Llink lookUpStopRoute(Llink head, stop * stop);
#endif