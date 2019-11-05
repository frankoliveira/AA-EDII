#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependente.h"

Dependente* criarDependente(char* n, int idade, int cod_emp){
	int static cod = 0;
    Dependente* e = malloc(tamanhoDependente());
    e->cod = cod;
    cod++;
    strcpy(e->nome, n);
    e->idade = idade;
    e->cod_emp = cod_emp;
    e->status = 1;
    return e;
}

void salva_depend(Dependente *e, FILE *out){
	if(out != NULL){
        fwrite(&e->cod, sizeof(int), 1, out);
        fwrite(e->nome, sizeof(char), sizeof(e->nome), out);
        fwrite(&e->idade, sizeof(int), sizeof(e->idade), out);
        fwrite(&e->cod_emp, sizeof(int), sizeof(e->cod_emp), out);
        fwrite(&e->status, sizeof(int), sizeof(e->status), out);
    }
    else    printf("File passado é null");
    
    free(e);
}

Dependente* le_depend(FILE *in){
	Dependente *e = (Dependente*) malloc(sizeof(Dependente));
    if (0 >= fread(&e->cod, sizeof(int), 1, in))
    {
        free(e);
        return NULL;
    }
    fread(e->nome, sizeof(char), sizeof(e->nome), in);
    fread(&e->idade, sizeof(int), sizeof(e->idade), in);
    fread(&e->status, sizeof(int), sizeof(e->status), in);
    fread(&e->cod_emp, sizeof(int), sizeof(e->cod_emp), in);
    return e;
}

void imprime_depend(Dependente *e){
    printf("\n\nDependente:\nCod: %d\nNome: %s\nIdade:%d\nCod Empregado:%d\nStatus:%d\n", (e->cod), e->nome, e->idade, e->cod_emp, e->status);
}

int tamanhoDependente(){
	return (sizeof(int) + 50*sizeof(char) + sizeof(int) + sizeof(int));
}
