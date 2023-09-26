/*  
structs.c
Teresa Margarida Vargas de Matos ist 1106009
Ficheiro que manipula paragens e carreiras
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "structs.h"

#define NO_MEMORY 1

/*função malloc, acaba o programa se o ponteiro criado for NULL ou seja se não 
haver mais memória disponível*/
void *MyMalloc(unsigned int size){
    void *p = malloc(size);
    if(p != NULL)
        return p;
    printf("No memory.\n");
    exit(NO_MEMORY);
}

/*compara a latitude e longitude de duas paragens e retorna TRUE se forem 
iguais*/
int compareStops(stop* stop1, stop* stop2){
    return (stop1->latitude == stop2->latitude
    && stop1->longitude == stop2->longitude);
}

/*recebe a cabeça da lista de carreiras e uma string e devolve o ponteiro para o link da carreira caso esta exista, devolve NULL senão */
Clink lookUpline(Clink head, char *nome){
    Clink ite = head;
    if(head == NULL) return NULL;
    while(ite != NULL){
        if(!strcmp(ite->line->nome, nome))
            return ite;
        ite = ite->next;
    }
    return NULL;
}

/*recebe a cabeça da lista de paragens e uma string e devolve o ponteiro para o 
link da paragem caso esta exista, devolve NULL senão */
Plink lookUpstop(Plink head, char *nome){
    Plink ite = head;
    if(head == NULL) return NULL;
    while(ite != NULL){
        if(!strcmp(ite->stop->nome, nome))
            return ite;
        ite = ite->next;
    }
    return NULL;
}


/*recebe um ponteiro para a cabeça da lista da carreiras e uma carreira e 
adiciona esta no fim*/
Clink insertNodeline(Clink head, line * line){
    Clink new = (Clink)MyMalloc(sizeof(Cnode));
    new->line = line;
    if(head == NULL){
        new->next = NULL;
        new->prev = new;
        return new;
    }
    new->next = NULL; 
    new->prev = head->prev; 
    if(head->next == NULL)
        head->next = new;
    else
        head->prev->next = new;
    head->prev = new; 
    return head;
}

/*recebe um ponteiro para a cabeça da lista de paragens e uma paragem e 
adiciona esta no fim*/
Plink insertNodestop(Plink head, stop * stop){
    Plink new = (Plink)MyMalloc(sizeof(Pnode));
    new->stop = stop;
    if(head == NULL){
        new->next = NULL;
        new->prev = new;
        return new;
    }
    new->next = NULL;
    new->prev = head->prev;
    if(head->next == NULL)
        head->next = new;
    else
        head->prev->next = new;
    head->prev = new;
    return head;
}



/*remove carreira da lista e retorna a cabeça*/
Clink removeNodeline(Clink line, Clink Headline){
    if(line == Headline){ /*se primeira*/
        if(line->next){
            line->next->prev = line->prev;
            return line->next;
        }else{
            return NULL;
        }
    }else if(line->next == NULL){/*se for ultima*/
        line->prev->next = NULL;
        Headline->prev = line->prev;
        return Headline;
    }
    line->prev->next = line->next;
    line->next->prev= line->prev;
    return Headline;
}

/*remove paragem da lista e retorna a cabeça*/
Plink removeNodestop(Plink stop, Plink Headstop){
    if(stop == Headstop){ /*se primeira*/
        if(stop->next){
            stop->next->prev = stop->prev;
            return stop->next;
        }else{
            return NULL;
        }
    }else if(stop->next == NULL){/*se for ultima*/
        stop->prev->next = NULL;
        Headstop->prev = stop->prev;
        return Headstop;
    }
    stop->prev->next = stop->next;
    stop->next->prev= stop->prev;
    return Headstop;
}

/*cria uma paragem*/
stop * Createstop(char * nome, double lat, double longi){
    stop * new = (stop *)MyMalloc(sizeof(stop));
    new->nome = nome;
    new->latitude = lat;
    new->longitude = longi;
    /*free(nome);*/
    return new;
}

/*cria uma carreira */
line * Createline(char * nome){
    line * new = (line *)MyMalloc(sizeof(line));
    new->nome = nome;
    new->head = NULL;
    new->tail = NULL;
    new->cost = 0;
    new->duration = 0;
    new->nPara = 0;
    return new;
}


void longlat(stop * stop){
    printf("%16.12f %16.12f\n", stop->latitude, stop->longitude);
}

/*apaga o primeiro elemento e retorna a nova head*/
Clink popline(Clink head){
    Clink newhead;
    if (!head) return NULL;
    newhead = head->next;
    free(head);
    return newhead;
}


/*apaga o primeiro elemento e retorna a nova head*/
Plink popstop(Plink head){
    Plink newhead;
    if (!head) return NULL;
    newhead = head->next;
    free(head->stop->nome);
    free(head->stop);
    free(head);
    return newhead;
}


/*apaga o primeiro elemento e retorna a nova head*/
Llink popRoute(Llink head){
    Llink newhead;
    if (!head) return NULL;
    newhead = head->next;
    newhead->prev = NULL;
    free(head);
    return newhead;
}

/*apaga percurso de uma carreira*/
line * DeleteRoute(line * line){
    Llink ite;
    if (!line) return NULL;
    ite = line->head;
    if(!ite)
        return line;
    while(ite->next != NULL)
        ite = popRoute(ite);
    free(ite);
    return line;
}

/*apaga uma carreira*/
void Deleteline(line * line){
    DeleteRoute(line);
    free(line->nome);
    free(line);
}

/*conta as paragens numa carreira*/
int countlines(Clink Headlines){
    Clink ite = Headlines;
    int nlines = 0;
    while(ite != NULL){
        nlines++;
        ite = ite->next;
    }
    return nlines;
}

/*reinicia uma carreira*/
void resetline(line * line){
    DeleteRoute(line);
    line->head = NULL;
    line->tail = NULL;
    line->cost = 0;
    line->duration = 0;
    line->nPara = 0;
}

