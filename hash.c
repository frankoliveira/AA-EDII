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

void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int l, int *qtd_registros){
	int chave = hash(emp->cod, tam, l);
	int excl, aux;
	Empregado *emp_aux;
	while(fread(&excl, sizeof(int), 1, exclusao) != 0){ //Procurar caso tenha algum registro excluido
		if(excl != -1) break;
	}
	fseek(h, chave*tamanhoEmpregado(), SEEK_SET);
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
			fwrite(&qtd_registros, sizeof(int), 1, h);
			qtd_registros++;
		}
	}
	else{
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
		}
		else{
			fseek(r, 0, SEEK_END);
			salva_empreg(emp, r);
			emp_aux->prox = qtd_registros;
			qtd_registros++;
		}
	}
}

int expandHash(FILE *h, FILE *r, int p, int tam, int l){
	int j = -1;
	int aux;
	fseek(h, 0, SEEK_END);
	fwrite(&j, sizeof(int), 1, h);
	//Organizar hash
	fseek(h, p*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux == -1) //Se na posição p da hash estiver vazio não precisa reorganizar
		return 1;
	else{
		Empregado *emp;
		int nova_chave, run = 1;
		int chave_atual = -1;
		int chave_ant_foi = -1;
		int chave_ant_ficou = -1;
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp = le_empreg(r);
		while(run == 1){
			nova_chave = hash(emp->cod, tam, l+1);
			if(nova_chave != p){ //Se a nova chave for diferente da posição p da hash significa q ela precisa ser deslocado pra a expansão
				fseek(h, nova_chave*sizeof(int), SEEK_SET);
				fread(&aux, sizeof(int), 1, h);
				if(aux == -1){ //Caso a noa partição extendida da hash esteja vazia
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&emp-cod, sizeof(int), 1, h);
					chave_ant_foi = nova_chave;
				}
				else{
					fseek(r, chave_ant_foi*tamanhoEmpregado(), SEEK_SET);
					emp = le_empreg(r);
					emp->prox = chave_atual;
					fseek(r, chave_ant_foi*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp, r);
					chave_ant_foi = nova_chave;
				}
			}
			else{
				if(chave_ant_ficou == -1){ //Caso seja a primeira chave a continuar em p
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&emp-cod, sizeof(int), 1, h);
					chave_ant_ficou = nova_chave;
				}
				else{
					fseek(r, chave_ant_ficou*tamanhoEmpregado(), SEEK_SET);
					emp = le_empreg(r);
					emp->prox = chave_atual;
					fseek(r, chave_ant_ficou*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp, r);
					chave_ant_ficou = nova_chave;
				}
			}
			if(emp->prox != -1){ //Continua até o final da lista de p
				chave_atual = emp->prox;
				fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
				emp = le_empreg(r);
			}
			else{
				run = 0;
			}
		}
	}
}
