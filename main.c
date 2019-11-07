#include<stdio.h>
#include<stdlib.h>
#include "empregado.c"
#include "hash.c"

int main(){
    int* qtd_reg = (int*) malloc(sizeof(int));
    *qtd_reg = 0;
    FILE* hash = fopen("hash.dat", "w+b");
    FILE* emp = fopen("empregado.dat", "w+b");
    FILE* exc = fopen("excluidos.dat", "w+b");
    
    Empregado* Joana = criarEmpregado("Joana", 18, 1650.0);
    imprime_empreg(Joana);
    Empregado* Joao = criarEmpregado("Joao", 24, 1000.0);
    imprime_empreg(Joao);
    
    initHash(hash, 5);
    
    imprimeHash(hash);
    inserirHash(hash, emp, exc, Joana, 5, 0, qtd_reg);
    printf("Inserimos Joana...\n\n HASH :\n");
    imprimeHash(hash);   // Segunda impressão bugada -> Provavelmente, inserirHash c problemas!!
    
    printf("Na main\n");
    
    return 0;
}
