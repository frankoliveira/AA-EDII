#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empregado.h"

Empregado* criarEmpregado(char* n, int idade, double sal){
    int static cod = 1;
    Empregado* e = (Empregado *) malloc(sizeof(Empregado));
    if (e) memset(e, 0, sizeof(Empregado));
    e->cod = cod;
    cod++;
    strcpy(e->nome, n);
    e->idade = idade;
    e->salario = sal;
    e->n_dependentes = 0;
    e->prox = -1;
    e->dependentes = -1;
    e->status = 1;
    return e;
}

void salva_empreg(Empregado *e, FILE *out){
    if(out != NULL){
        fwrite(&e->cod, sizeof(int), 1, out);
        fwrite(e->nome, sizeof(char), sizeof(e->nome), out);
        fwrite(&e->idade, sizeof(int), 1, out);
        fwrite(&e->salario, sizeof(double), 1, out);
        fwrite(&e->n_dependentes, sizeof(int), 1, out);
        fwrite(&e->prox, sizeof(int), 1, out);
        fwrite(&e->dependentes, sizeof(int), 1, out);
        fwrite(&e->status, sizeof(int), 1, out);
    }
    else    printf("File passado é null");
    
    free(e);
}

Empregado* le_empreg(FILE *in){
    Empregado *e = (Empregado*) malloc(sizeof(Empregado));
    if (0 >= fread(&e->cod, sizeof(int), 1, in))
    {
        free(e);
        return NULL;
    }
    fread(e->nome, sizeof(char), sizeof(e->nome), in);
    fread(&e->idade, sizeof(int), 1, in);
    fread(&e->salario, sizeof(double), 1, in);
    fread(&e->n_dependentes, sizeof(int), 1, in);
    fread(&e->prox, sizeof(int), 1, in);
    fread(&e->dependentes, sizeof(int), 1, in);
    fread(&e->status, sizeof(int), 1, in);
    return e;
}

void imprime_empreg(Empregado *e){
    printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\nProx:%d\nStatus:%d\n", e->cod, e->nome, e->idade, e->salario, e->n_dependentes, e->prox, e->status);
}

int tamanhoEmpregado(){
    return sizeof(int)      //cod
        + sizeof(char) * 50 //nome
        + sizeof(int)       //idade
        + sizeof(double)    //salario
        + sizeof(int)       //n_dependentes
        + sizeof(int)       //status
        + sizeof(int)       //dependentes
        + sizeof(int);      //prox
}
