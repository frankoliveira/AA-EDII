#include <stdio.h>
#include <stdlib.h>
#include "dependente.h"
//#include "empregado.h"

void initHash(FILE *h, int tam);
int hash(int n, int tam, int l);
void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int l, int *qtd_registros);
void expandHash(FILE *h, FILE *r, int p, int tam, int l);
int busca_por_cod(FILE *hash, FILE* regts, int cod, int tam, int l);
