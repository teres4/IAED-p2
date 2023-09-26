/*  
ligacoes.c
Teresa Margarida Vargas de Matos ist 1106009
Ficheiro que manipula ligações
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "structs.h"
#include "ligacoes.h"

/*Recebe uma carreira, paragem de origem e destino e devolve o tipo de link que 
é feito*/
int connectionType(line * line, stop * origem, stop * destino){
    if(line->head == NULL && line->tail == NULL)
        return TRUE;
    /*carreira sem paragens*/
    else if(compareStops(line->tail->stop, origem) && 
    compareStops(line->head->stop, destino))
        return CICLO;
    /*carreira circular*/
    else if(compareStops(line->head->stop, destino))
        return ORIGEM;
    /*adicionar no inicio*/
    else if(compareStops(line->tail->stop, origem))
        return DESTINO;
    /*adicionar no fim*/
    return FALSE;
    
}

/*Adiciona uma paragem a uma carreira no ínicio do percurso*/
line* changeHead(line * line, stop *origem, double cost, double duration){
    Llink new = (Llink)MyMalloc(sizeof(Lnode));
    new->stop = origem;
    new->nextcost = cost;
    new->nextduration = duration;
    new->prevcost = 0;
    new->prevduration = 0;
    if(line->head == NULL){
        line->head = new;
        new->next = NULL;
        new->prev = NULL;
        addcounterC(line);
        return line;
    }
    new->stop = origem;
    line->head->prev = new;
    new->next = line->head;
    new->next->prevcost = cost;
    new->next->prevduration = duration;
    line->head = new;
    addcounterC(line);
    return line;
}

/*Adiciona uma paragem a uma carreira no fim do percurso*/
line* changeTail(line * line, stop *destino, double cost, double duration){
    Llink new = (Llink)MyMalloc(sizeof(Lnode));
    new->stop = destino;
    new->prevcost = cost;
    new->prevduration = duration;
    new->nextcost = 0;
    new->nextduration = 0;
    if(line->tail == NULL){
        line->tail = new;
        new->next = NULL;
        new->prev = line->head;
        line->head->next = new;
        addcounterC(line);
        return line;
    }
    line->tail->next = new;
    new->prev = line->tail;
    new->prev->nextcost = cost;
    new->prev->nextduration = duration;
    line->tail = new;
    new->next  = NULL;
    addcounterC(line);
    return line;
}

/*incrementa o número de paragens numa carreira*/
line* addcounterC(line * line){
    line->nPara++;
    return line;
}

/*recebe uma carreira e mostra as caracteristicas desta*/
void listlines(line * line){
    if (line == NULL) return;
    if(line->nPara != 0){
        printf("%s %s %s %d %.2f %.2f\n",
        line->nome,
        line->head->stop->nome,
        line->tail->stop->nome,
        line->nPara,
        line->cost,
        line->duration);
    }else{
        printf("%s %d %.2f %.2f\n",
        line->nome,
        0, 0.0, 0.0);
        }
    
}

/*Recebe uma carreira e mostra o percurso desta, começando pelo início*/
void listRoute(line * line){
    Llink ite;
    if(line->nPara != 0){
        ite = line->head;
        printf("%s, ", ite->stop->nome);
        ite = ite->next;
        while(ite){
            if(ite->next == NULL){
                printf("%s\n", ite->stop->nome);
                break;
            }
            printf("%s, ", ite->stop->nome);
            ite = ite->next;
        }
    }
}

/*Recbe uma carreira e mostra o percurso desta, começando pelo fim*/
void listRouteInv(line * line){
    Llink ite;
    if(line->nPara != 0){
        ite = line->tail;
        printf("%s, ", ite->stop->nome);
        ite = ite->prev;
        while(ite){
            if(ite->prev == NULL){
                printf("%s\n", ite->stop->nome);
                break;
            }
            printf("%s, ", ite->stop->nome);
            ite = ite->prev;
        }
    }
}

/*recebe uma paragen e mostra as características desta*/
void listStops(stop * stop, Clink Headlines){
    int nlines = getNlines(stop, Headlines);
    if(stop == NULL) return;
    printf("%s: %16.12f %16.12f %d\n",
    stop->nome,
    stop->latitude,
    stop->longitude,
    nlines);
}

/*Atualiza uma carreira de acordo com o tipo de mudanca*/
void UpdateLine(line * line, stop * origem, stop * destino, double cost, double duration, int mudanca){
    switch(mudanca){
        case(TRUE):
            /*carreira vazia*/
            changeHead(line, origem, cost, duration);
            changeTail(line, destino, cost , duration);
            break;
        case(DESTINO):
            changeTail(line, destino, cost, duration);
            break;
        case(ORIGEM):
            changeHead(line, origem, cost, duration);
            break;
        case(CICLO):
            changeTail(line, destino, cost, duration);
            break;
    }
    line->cost += cost;
    line->duration += duration;

}

/*Devolve o número que carreiras que passam numa paragem*/
int getNlines(stop * stop, Clink  Headlines){
    Clink ite =  Headlines;
    Llink iteP;
    int n = 0;
    if (!Headlines) return 0;
    while(ite != NULL){
        iteP = ite->line->head;
        while(iteP != NULL){
            if(iteP->stop == stop){
                n++;
                break;
            }
            iteP = iteP->next;
        }
        ite = ite->next;
    }
    return n;
}


void swap(line ** vec, int i, int j) {
    line* temp = vec[i];
    vec[i] = vec[j];
    vec[j] = temp;
}

/*algoritmo de ordenação Selection Sort, modificado para ordenar pointers para 
carreiras de acordo com os seus nomes*/
void selectionsort(line **vec, int n){
    int i, j, imin;
    for (i = 0; i < n-1; i++) {
        imin = i;
        for (j = i+1; j < n; j++) {
            if (strcmp(vec[j]->nome , vec[imin]->nome) < 0) {
                imin = j;
            }
        }
        swap(vec, imin, i);
    }
}

/*Recebe uma carreira e uma paragem e um ponteiro para um inteiro ,devolve o 
número de vezes que a paragem aparece na carreira e modifica o valor dentro do 
ponteiro para o numero de paragens na carreira*/
int nStopsinLine(line * line, stop * stop, int *ntotal){
    Llink ite;
    int n = 0;
    if(!line || !stop) return n;
    ite = line->head;
        while(ite != NULL){
            if(ite->stop == stop){
                n++;
            }
            ite = ite->next;
            (*ntotal)++;
        }
    return n;
}

/*Recebe uma carreira e devolve o número de paragens na carreira*/
int CountStops(line * line){
    Llink ite;
    int n = 0;
    if(!line) return n;
    ite = line->head;
        while(ite != NULL){
            n++;
            ite = ite->next;
        }
    return n;
}

/*Recebe uma carreira e uma paragem, apaga a paregem do percurso da carreira*/
void removeStopsinLine(line * line, stop * stop){
    int ntotal = 0, npara, i;
    npara = nStopsinLine(line, stop, &ntotal);
    if (!npara)
        return;
    else if(ntotal <= npara + 1){
        resetline(line);
        return;
    }
    for(i = 0;i < npara; i++)
        removeStopinRoute(line, stop);
    
    line->nPara = ntotal-npara;
}

/*Recebe uma carreira e uma paragem e remove-a do percurso, modifica também o 
custo e a duração total da carreira*/
void removeStopinRoute(line * line, stop * stop){
    Llink linkstop;
    if(line->head->stop == stop){
        line->cost -= line->head->nextcost;
        line->duration -= line->head->nextduration;
        line->head = line->head->next;
        free(line->head->prev);
        line->head->prev = NULL;
        return;
    }else if(line->tail->stop == stop){
        line->cost -= line->tail->prevcost;
        line->duration -= line->tail->prevduration;
        line->tail = line->tail->prev;
        free(line->tail->next);
        line->tail->next = NULL;
        return;
    }
    
    linkstop = lookUpStopRoute(line->head, stop);
    linkstop->prev->nextcost += linkstop->nextcost;
    linkstop->prev->nextduration += linkstop->nextduration;
    linkstop->next->prevcost += linkstop->prevcost;
    linkstop->next->prevduration += linkstop->prevduration;
    linkstop->next->prev = linkstop->prev;
    linkstop->prev->next = linkstop->next;
    free(linkstop);
    return;
}

/*recebe a cabeça do percurso e uma paragem e devolve o link a esta no percurso 
se pertencer, devolve NULL senão*/
Llink lookUpStopRoute(Llink head, stop * stop){
    Llink ite = head;
    while(ite != NULL){
        if(ite->stop == stop)
            return ite;
        ite = ite->next;
    }
    return NULL;
}