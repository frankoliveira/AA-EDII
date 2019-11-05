#include <stdio.h>
#include <stdlib.h>

typedef struct dependente{
    int cod;
    char* nome;
    int idade;
    int status;
    int cod_emp;
}Dependente;

Dependente* criarDependente(char* n, int idade, int cod_emp);
void salva_depend(Dependente *e, FILE *out);
Dependente* le_depend(FILE *in);
void imprime_depend(Dependente *e);
int tamanhoDependente();
