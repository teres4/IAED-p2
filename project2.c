
/*  
project2
Teresa Margarida Vargas de Matos ist 1106009
Ficheiro MAIN
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "structs.h"
#include "ligacoes.h"

#define MAXCOMMAND 65535 /*número de carateres numa instrução*/
#define FLOATSIZE 14 /*número máximo de carateres num float*/
#define INVERSESIZE 8  


struct Head{
    Clink lines;
    Plink stops;
}; 
typedef struct Head * LinkHead;

void comand_line(LinkHead Head, char * command);
void comand_connections(LinkHead Heads, char *command);
void comand_stops(LinkHead Head, char * command);
void comand_interligacoes(LinkHead Head);
void deleteline(LinkHead Head,char * command);
void interligacao(stop * stop, Clink Headline, int nlines);
void EraseAllData(LinkHead Head);
void deleteStop(LinkHead Head,char * command);
void handlelinks(LinkHead Heads, char *origem, char *destino, char *line, 
double custo, double duracao);
int controlcommands(LinkHead Head, char * command, int c);
int handle_commands(Clink Headline, Plink Headstop, char*command, int c);

/*Recebe uma string e devolve TRUE se for 'inverso' ou abreviatura deste, FALSE 
caso contrário*/
int IsInverse(char* inverso){
    int i, len = strlen(inverso);
    for(i = 0; i < len; i++)
        if((inverso[i] != "inverso"[i]) || (len < 3) || (len > 8))
            return FALSE;
    return TRUE;
}

/*Recebe uma string a preenche-a com caracteres do stdin até ao encontrar '\n'*/
void readLine(char * str) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i++] = '\n';
    str[i] = '\0';
}

/*função main*/
int main(){
    int c;
    LinkHead Head = (LinkHead)MyMalloc(sizeof(struct Head));
    char * command = (char *)MyMalloc(sizeof(char)*MAXCOMMAND);
    Head->lines = NULL;
    Head->stops  = NULL;
    while ((c = getchar()) != EOF && c != 'q')
        c = controlcommands(Head, command, c);
    free(command);
    EraseAllData(Head);
    free(Head);
    return 0;
}

/*função que lida com os comandos*/
int controlcommands(LinkHead Head, char * command, int c){
    readLine(command);
    switch(c){
        case 'q':
            return c;
        case 'c':
            comand_line(Head, command);
            return c;
        case 'p':
            comand_stops(Head, command);
            return c;
        case 'l':
            comand_connections(Head, command);
            return c;
        case 'i':
            comand_interligacoes(Head);
            return c;
        case 'r':
            deleteline(Head, command);
            return c;
        case 'a':
            EraseAllData(Head);
            return c;
        case 'e':
            deleteStop(Head,command);
            return c;
        }
    return c;
}

/*Recebe uma string str e preenche-a com carateres de command a partir do 
índice i até encontrar um espaço ou '\n', incrementa o índice.
Devolve TRUE se o último carater for '\n' ou seja se a função ler o último 
argumento*/
int getNextArg(char *str, char * command, int * i) {
    char c;
    int j = 0;
    c = command[(*i)++];
    if(c == '"'){
        c = command[(*i)++];
        do{
		str[j++] = c;
		c = command[(*i)++];}
        while (c != '"');
        c = command[(*i)++];
        str[j] = '\0';
        return (c == '\n');
	}
	while (c == ' ' || c == '\t')
		c = command[(*i)++];
	while (c != ' ' && c != '\t' && c != '\n') {
		str[j++] = c;
		c = command[(*i)++];
	}
	str[j] = '\0';
	return (c == '\n');
}

/*Recebe uma string e devolve o double lido a partir do índice i*/
double getNextNumber(char * command, int * i){
    char c;
    char str[FLOATSIZE];
    double number;
    int j = 0;
    c = command[(*i)++];
	while (c == ' ' || c == '\t')
		c = command[(*i)++];
	while (c != ' ' && c != '\t' && c != '\n') {
		str[j++] = c;
		c = command[(*i)++];
	}
	str[j] = '\0';
    number = atof(str);
	return number;
}

/*Recebe uma string e devolve o tamanho do próximo argumento a partir de i*/
int nextArgLen(char *command, int i){
    int j = 0;
    if(command[i] == '"'){
        (i)++;
        do{
		(i)++;
        j++;
        }
        while (command[i] != '"');
    }
    while (command[i] == ' ' || command[i] == '\t')
		(i)++;
    while (command[i] != ' ' && command[i] != '\t' && command[i] != '\n') {
		(i)++;
		j++;
	}
    return j;
}

/*Função que lida com o comando 'q'*/
void comand_line(LinkHead Head, char * command){
    int lastarg, i = 0;
    Clink aux = Head->lines, Linkline;
    char a = command[i], inverso[INVERSESIZE];
    char * nome;
    if ((a == '\n') || (a == EOF) || (a == 0)){ 
        /*comando c sem argumentos*/
        if (!Head->lines) return;
        while(aux != NULL){
            listlines(aux->line);
            aux = aux->next;
            }
        return;
    }
    nome = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    lastarg = getNextArg(nome, command, &i);
    Linkline = lookUpline(Head->lines, nome);
    if(Linkline != NULL){
        if(lastarg){
            /*comando c seguido de uma carreira existente*/
            listRoute(Linkline->line);
        }else{
            getNextArg(inverso, command, &i);
            if(IsInverse(inverso))
                listRouteInv(Linkline->line);
            else
                printf("incorrect sort option.\n");
        }
    }else
        /*criação e inserção de nova carreira*/
        Head->lines = insertNodeline(Head->lines, Createline(nome));
}


/*Função que lida com o comando 'p'*/
void comand_stops(LinkHead Head, char * command){
    int i = 0;
    Plink aux = Head->stops, Linkstop;
    char a = command[i], lastarg, *nome;
    double latitude, longitude;
    if ((a == '\n') || (a == EOF) || (a == 0)){ 
        /*comando p sem argumentos*/
        while(aux != NULL){
            listStops(aux->stop, Head->lines);
            aux = aux->next;
            }
        return;
    }
    nome = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    lastarg = getNextArg(nome, command, &i);
    Linkstop = lookUpstop(Head->stops, nome);
    if(lastarg){
        /*comando p seguido só de a paragem*/
        if(Linkstop != NULL)
            longlat(Linkstop->stop);
        else
            printf("%s: no such stop.\n", nome);
    }else{
        if(Linkstop != NULL)
            printf("%s: stop already exists.\n", nome);
        else{
            /*criação e inserção de nova paragem*/
            latitude = getNextNumber(command, &i);
            longitude = getNextNumber(command, &i);
            Head->stops = insertNodestop(Head->stops,Createstop
            (nome, latitude, longitude));
        }
    }
}

/*Função que lida com o comando 'l'*/
void comand_connections(LinkHead Heads, char *command){
    int i = 0;
    char *origem, *destino, *line;
    double custo, duracao;
    line = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    getNextArg(line, command, &i);
    origem = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    getNextArg(origem, command, &i);
    destino = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    getNextArg(destino, command, &i);
    custo = getNextNumber(command, &i);
    duracao = getNextNumber(command, &i);
    handlelinks(Heads, origem, destino, line, custo, duracao);
    free(line);
    free(origem);
    free(destino);
}

/*atualiza e gere ligações*/
void handlelinks(LinkHead Heads, char *origem, char *destino, char *line, 
double custo, double duracao){
    int mudanca;
    Clink linkline;
    Plink linkorigem, linkdestino;
    if(!(linkline = lookUpline(Heads->lines, line))){
        printf("%s: no such line.\n", line);
        return;
    }else if(!(linkorigem = lookUpstop(Heads->stops,origem))){
        printf("%s: no such stop.\n", origem);
        return;
    }else if(!(linkdestino = lookUpstop(Heads->stops,destino))){
        printf("%s: no such stop.\n", destino);
        return;
    }else if(!(mudanca = connectionType(linkline->line, linkorigem->stop, 
    linkdestino->stop))){
        printf("link cannot be associated with bus line.\n");
        return; 
    }else if((custo < 0) || (duracao < 0)){
        printf("negative cost or duration.\n");
        return;
    }
    UpdateLine(linkline->line, linkorigem->stop, 
    linkdestino->stop, custo, duracao, mudanca);
}

/*Função que lida com o comando 'r'*/
void deleteline(LinkHead Head, char * command){
    int i = 0;
    char * nome;
    Clink linkline;
    nome = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    getNextArg(nome, command, &i);
    linkline = lookUpline(Head->lines, nome);
    if(!linkline){
        printf("%s: no such line.\n", nome);
        free(nome);
        return;
    }
    Deleteline(linkline->line);
    Head->lines = removeNodeline(linkline, Head->lines);
    free(linkline);
    free(nome);
    return;
}

/*função que lida com o comando 'i'*/
void comand_interligacoes(LinkHead Heads){
    Plink aux = Heads->stops;
    int nlines = countlines(Heads->lines);
    while(aux != NULL){
        interligacao(aux->stop, Heads->lines, nlines);
        aux = aux->next;
    }
}

/*Recebe uma paragem e mostra as carreiras que passam nesta, cajo seja uma 
interseção*/
void interligacao(stop * stop, Clink Headline, int nlines){
    line ** vec = (line **)MyMalloc(sizeof(line*)*nlines);
    Clink aux =  Headline;
    Llink auxP;
    int nInter = 0, i = 0;
    if (!Headline || !stop) return;
    while(aux != NULL){
        /*adicionar ponteiros de carreiras que passam na paragem ao vetor*/
        auxP = aux->line->head;
        while(auxP != NULL){
            if(auxP->stop == stop){
                vec[nInter++] = aux->line;
                break;
            }
            auxP = auxP->next;
        }
        aux = aux->next;
    }

    if(nInter > 1){
        selectionsort(vec, nInter);
        printf("%s %d: " , stop->nome, nInter);
        for(i = 0; i < nInter - 1 ; i++)
            printf("%s ", vec[i]->nome);
        printf("%s\n", vec[i]->nome);
    }
    free(vec);
}

/*Função que lida com o comando 'a'*/
void EraseAllData(LinkHead Head){
    Clink aux = Head->lines;
    Plink auxP = Head->stops;
    if(aux){
        while(aux->next != NULL){
            Deleteline(aux->line);
            aux = popline(aux);
        }
        free(aux);
    }
    if(auxP){
        while(auxP->next != NULL)
            auxP = popstop(auxP);
        free(auxP);
    }
    Head->lines = NULL;
    Head->stops = NULL;
}

/*Função que lida com o comando 'e'*/
void deleteStop(LinkHead Heads,char * command){
    int i = 0;
    char * nome;
    Plink linkstop;
    Clink aux = Heads->lines;
    nome = (char *)MyMalloc((sizeof(char))*(nextArgLen(command, i) + 1));
    getNextArg(nome, command, &i);
    linkstop = lookUpstop(Heads->stops, nome);
    if(!linkstop){
        printf("%s: no such stop.\n", nome);
        free(nome);
        return;
    }
    Heads->stops = removeNodestop(linkstop, Heads->stops);
    while(aux){
        removeStopsinLine(aux->line, linkstop->stop);
        aux = aux->next;
    }
    free(linkstop->stop->nome);
    free(linkstop->stop);
    free(linkstop);
    free(nome);
    return;
}