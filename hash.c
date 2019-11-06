#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dependente.c"
#include "empregado.c"

void initHash(FILE *h, int tam){
	int j = -1;
	for (int i = 0; i < tam; ++i){
		fwrite(&j, sizeof(int), 1, h);
	}
}

int hash(int n, int tam, int l){
    return (n%(tam*(int)pow(2,l)));
}

void imprimeHash(FILE *hash){
	int aux;
	rewind(hash);
	while(fread(&aux, sizeof(int), 1, hash) != 0){
		printf("%d\n", aux);
	}
}

void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int l, int *qtd_registros){
	int chave = hash(emp->cod, tam, l);
	int excl, aux;
	rewind(exclusao);
	while(fread(&excl, sizeof(int), 1, exclusao) != 0){ //Procurar caso tenha algum registro excluido
		if(excl != -1) break;
	}
	fseek(h, chave*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux == -1){ //Caso a chave da hash esteja vazia
		fseek(h, chave*tamanhoEmpregado(), SEEK_SET);
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp, r);
			fwrite(&excl, sizeof(int), 1, h);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_empreg(emp, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&(*qtd_registros), sizeof(int), 1, h);
			*qtd_registros += 1;
		}
	}
	else{
		Empregado *emp_aux;
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp_aux = le_empreg(r);
		while(emp_aux->prox != -1){ //Vai até o final da lista encadeada
			fseek(r, emp_aux->prox*tamanhoEmpregado(), SEEK_SET);
			emp_aux = le_empreg(r);
		}
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp, r);
			emp_aux->prox = excl;
			fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_empreg(emp, r);
			emp_aux->prox = *qtd_registros;
			fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
			*qtd_registros += 1;
		}
	}
}

void expandHash(FILE *h, FILE *r, int tam, int p, int l){
	int j = -1;
	int aux;
	fseek(h, 0, SEEK_END);
	fwrite(&j, sizeof(int), 1, h);
	//Organizar hash
	fseek(h, p*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux != -1){ //Se na posição p da hash não estiver vazio
		Empregado *emp;
		int nova_chave;
		int end_atual = -1;
		int end_foi = -1;
		int end_ficou = -1;
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp = le_empreg(r);
		do{
			nova_chave = hash(emp->cod, tam, l+1);
			if(nova_chave != p){ //Se a nova chave for diferente da posição p da hash significa q ela precisa ser deslocado pra a expansão
				fseek(h, nova_chave*sizeof(int), SEEK_SET);
				fread(&aux, sizeof(int), 1, h);
				if(aux == -1){ //Caso a noa partição extendida da hash esteja vazia
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&emp->cod, sizeof(int), 1, h);
					end_foi = nova_chave;
				}
				else{
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					emp = le_empreg(r);
					emp->prox = end_atual;
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp, r);
					end_foi = nova_chave;
				}
			}
			else{
				if(end_ficou == -1){ //Caso seja a primeira chave a continuar em p
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&emp->cod, sizeof(int), 1, h);
					end_ficou = nova_chave;
				}
				else{
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					emp = le_empreg(r);
					emp->prox = end_atual;
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp, r);
					end_ficou = nova_chave;
				}
			}
			if(emp->prox != -1){ //Continua até o final da lista de p
				end_atual = emp->prox;
				fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
				emp = le_empreg(r);
			}
		}while(emp->prox != -1);
	}
}
