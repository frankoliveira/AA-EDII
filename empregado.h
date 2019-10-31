#include<stdio.h>
#include<stdlib.h>

typedef struct empregado{
    int cod;
    char nome[50];
    int idade;
    double salario;
    int n_dependentes;
    // Atributos para a hash extensível com encadeamento exterior
    int prox;
    int status;
}Empregado;

typedef struct dependente{
    int cod;
    char* nome;
    int idade;
    int cod_emp;
}Dependente;

Empregado* criarEmpregado(char* n, int idade, double sal, int n_dep);
void salva(Empregado *e, FILE *out);
Empregado* le(FILE *in);
void le_tudo(FILE *in);
int tamanhoEmpregado();
