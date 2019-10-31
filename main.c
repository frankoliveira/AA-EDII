#include<stdio.h>
#include<stdlib.h>
#include "empregado.h"
#include "hash.h"

int main(){
    FILE* emp = fopen("empregado.dat", w+b);
    Empregado* Joana = criarEmpregado("Joana", 18, 1650.0, 0);
    
    //Aki, teremos as funções de hash extensível
    
    salva(Joana, emp);
    return 0;
}
