#include "empregado.h"
#include<string.h>

Empregado* criarEmpregado(char* n, int idade, double sal, int n_dep){
    int static cod = 0;
    Empregado* e = malloc(tamanhoEmpregado());
    e->cod = cod;
    cod++;
    strcpy(e->nome, n);
    e->idade = idade;
    e->salario = sal;
    e->n_dependentes = n_dep;
    e->prox = -1;
    e->status = 1;
    return e;
}

void salva(Empregado *e, FILE *out){
    if(out != NULL){
        fwrite(&e->cod, sizeof(int), 1, out);
        fwrite(e->nome, sizeof(char), sizeof(e->nome), out);
        fwrite(&e->idade, sizeof(int), sizeof(e->idade), out);
        fwrite(&e->salario, sizeof(double), sizeof(e->salario), out);
        fwrite(&e->n_dependentes, sizeof(int), sizeof(e->n_dependentes), out);
        fwrite(&e->prox, sizeof(int), sizeof(e->prox), out);
        fwrite(&e->status, sizeof(int), sizeof(e->status), out);
    }
    else    printf("File passado é null");
    
    free(e);
}

Empregado* le(FILE *in){
    Empregado *e = (Empregado*) malloc(sizeof(Empregado));
    if (0 >= fread(&e->cod, sizeof(int), 1, in))
    {
        free(e);
        return NULL;
    }
    fread(e->nome, sizeof(char), sizeof(e->nome), in);
    fread(&e->idade, sizeof(int), sizeof(e->idade), in);
    fread(&e->salario, sizeof(double), sizeof(e->salario), in);
    fread(&e->n_dependentes, sizeof(int), sizeof(e->n_dependentes), in);
    fread(&e->prox, sizeof(int), sizeof(e->prox), in);
    fread(&e->status, sizeof(int), sizeof(e->status), in);
    return e;
}

void le_tudo(FILE *in){
    rewind(in);	//voltando para o início do arq
    Empregado *e = malloc(sizeof(Empregado));
    //printf("bytes lidos = %ld", fread(&c->cod, sizeof(int), 1, in));    //seg fault no fread !!!
    while(fread(&e->cod, sizeof(int), 1, in) > 0){
        fread(e->nome, sizeof(char), sizeof(e->nome), in);
        fread(&e->idade, sizeof(int), sizeof(e->idade), in);
        fread(&e->salario, sizeof(double), sizeof(e->salario), in);
        fread(&e->n_dependentes, sizeof(int), sizeof(e->n_dependentes), in);
        fread(&e->prox, sizeof(int), sizeof(e->prox), in);
        fread(&e->status, sizeof(int), sizeof(e->status), in);
        printf("\n\nEmpregado:\nCod: %d\nNome: %s\nIdade:%d\nSalario:%f\nNº dependentes:%d\nProx:%d\nStatus:%d\n", (e->cod), e->nome, e->idade, e->salario, e->n_dependentes, e->prox, e->status);
    }
}

int tamanhoEmpregado(){
    return (sizeof(int) + 50*sizeof(char) + sizeof(int) + sizeof(double) + sizeof(int));
}
