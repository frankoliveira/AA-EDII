#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dependente.c"
#include "empregado.c"
#define fbMax 1.1

void expandHash(FILE *h, FILE *r, int tam, int* p, int l);

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

void imprime_reg(FILE* r){
    rewind(r);
    Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
    imprime_empreg(le_empreg(r));
}

void percorrendo_lista(FILE* h, FILE* reg, int r_hash){
    Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
    int aux = -1;
    fseek(h, r_hash*sizeof(int), SEEK_SET);
    fread(&aux, sizeof(int), 1, h);
    
    fseek(reg, aux*tamanhoEmpregado(), SEEK_SET);
    e = le_empreg(reg);
    if(e->status != -1)   imprime_empreg(e);
    while(e->prox != -1){
        fseek(reg, e->prox*tamanhoEmpregado(), SEEK_SET);
        e = le_empreg(reg);
        if(e->status != -1)    imprime_empreg(e);
    } 
    free(e);
}

void inserirHash(FILE *h, FILE *r, FILE *exclusao, Empregado *emp, int tam, int* p, int l, int *qtd_registros){
	int chave = hash(emp->cod, tam, l);
	int excl, aux;
	rewind(exclusao);
	while(fread(&excl, sizeof(int), 1, exclusao) != 0){ //Procurar caso tenha algum registro excluido
		if(excl != -1) break;
	}
	fseek(h, chave*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
    printf("Chegamos aqui. cod %d\n", emp->cod);
	if(aux == -1){ //Caso a chave da hash esteja vazia
        printf("Hash vazia\n");
		fseek(h, chave*tamanhoEmpregado(), SEEK_SET);
		if(excl != -1){ //Caso tenha um registro excluido
			fseek(r, excl*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp, r);
			fwrite(&excl, sizeof(int), 1, h);
		}
		else{
            printf("Indo pro final de r...\n");
			fseek(r, 0, SEEK_END);    // ----- Acho que isso q está dado erro
			salva_empreg(emp, r);
			fseek(h, chave*sizeof(int), SEEK_SET);
			fwrite(&(*qtd_registros), sizeof(int), 1, h);
			*qtd_registros += 1;
		}
	}
	else{
        printf("Hash não vazia\n");
		Empregado *emp_aux;
		int end_atual = aux;
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp_aux = le_empreg(r);
		while(emp_aux->prox != -1){ //Vai até o final da lista encadeada
			fseek(r, emp_aux->prox*tamanhoEmpregado(), SEEK_SET);
			end_atual = emp_aux->prox;
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
			fseek(r, end_atual*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
			*qtd_registros += 1;
		}
	}
    if((*qtd_registros)/(float)tam >= fbMax){
     
        expandHash(h, r, tam, p, l);
    }    
}

void expandHash(FILE *h, FILE *r, int tam, int* p, int l){
    int j = -1;
	int aux;
	//Expande a hash
	fseek(h, 0, SEEK_END);
	fwrite(&j, sizeof(int), 1, h);
	//Confere a posição de p na hash
	fseek(h, (*p)*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, h);
	if(aux != -1){ //Se na posição p da hash não estiver vazio
		//Coloca como vazia a posição de p
		fseek(h, (*p)*sizeof(int), SEEK_SET);
		fwrite(&j, sizeof(int), 1, h);

		Empregado *emp;
		int nova_chave;
		int end_atual = aux;
		int end_foi = -1;
		int end_ficou = -1;
		//Lê o registro q estava em p
		fseek(r, aux*tamanhoEmpregado(), SEEK_SET);
		emp = le_empreg(r);
		while(end_atual != -1){
			Empregado *emp_aux;
			nova_chave = hash(emp->cod, tam, l+1);
			if(nova_chave != *p){ //Se a nova chave for diferente da posição p da hash significa q ela precisa ser deslocado pra a expansão
				int end;
				fseek(h, nova_chave*sizeof(int), SEEK_SET);
				fread(&end, sizeof(int), 1, h);
				if(end_foi == -1){ //Caso a nova partição extendida da hash esteja vazia
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&aux, sizeof(int), 1, h);
					end_foi = end_atual;
				}
				else{
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					emp_aux = le_empreg(r);
					emp_aux->prox = end_atual;
					fseek(r, end_foi*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp_aux, r);
					end_foi = end_atual;
				}
			}
			else{
				if(end_ficou == -1){ //Caso seja a primeira chave a continuar em p
					fseek(h, nova_chave*sizeof(int), SEEK_SET);
					fwrite(&end_atual, sizeof(int), 1, h);
					end_ficou = end_atual;
				}
				else{
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					emp_aux = le_empreg(r);
					emp_aux->prox = end_atual;
					fseek(r, end_ficou*tamanhoEmpregado(), SEEK_SET);
					salva_empreg(emp_aux, r);
					end_ficou = end_atual;
				}
			}
			nova_chave = end_atual;
			end_atual = emp->prox;
			if(end_atual != -1){ //Continua até o final da lista de p
				fseek(r, emp->prox*tamanhoEmpregado(), SEEK_SET);
				emp = le_empreg(r);
			}
			fseek(r, nova_chave*tamanhoEmpregado(), SEEK_SET);
			emp_aux = le_empreg(r);
			emp_aux->prox = -1;
			fseek(r, nova_chave*tamanhoEmpregado(), SEEK_SET);
			salva_empreg(emp_aux, r);
		}
	}
    (*p)++;
}

int excluirHash(FILE *h, FILE *r, FILE *exclusao, int emp, int tam, int* p, int l, int *qtd_registros){
    int cod = 0, chave = 0, menos_um = -1;
    Empregado* e = (Empregado*) malloc(tamanhoEmpregado());
    Empregado* prox = (Empregado*) malloc(tamanhoEmpregado());

    fseek(r, emp*tamanhoEmpregado(), SEEK_SET);    // indo até registro em arq de reg
    if(fread(&cod, sizeof(int), 1, r) > 0){    //Conseguimos ler
   
        for(int i=0; i<=l; i++){    //rodando p/ cd l possível no momento em que o emp foi inserido
            chave = hash(cod, tam, i);
            //printf("Hash 1 = %d\n", chave);
            if(chave < *p){
                chave = hash(cod, tam+(*p), i);
                //printf("Hash 2 = %d\n", chave);
            }
            e = le_empreg(r);
            imprime_empreg(e);
            if(e->cod == cod){    //reg = 1º reg
                printf("   Primeiro   ...\n");   
                fseek(h, chave*sizeof(int), SEEK_SET);
                e->status = -1;
                fwrite(&emp, sizeof(int), 1, exclusao);
                if(e->prox != -1){    // tem proximo
                    fwrite(&e->prox, sizeof(int), 1, h);                    
                }
                else{
                    fwrite(&menos_um, sizeof(int), 1, h);
                }
                fseek(r, emp*tamanhoEmpregado(), SEEK_SET);
                salva_empreg(e, r);    // p/ salvar empreg com status -1
                free(e);    free(prox);
                return 1;
            }
            else{
                while(e->prox != -1){ //Vai até o final da lista encadeada
                    fseek(r, e->prox*tamanhoEmpregado(), SEEK_SET);
                    prox = le_empreg(r);    //lendo proximo
                    if(prox->cod == cod){    //achamos reg
                        e->prox = prox->prox;
                        prox->status = -1;
                        fwrite(&emp, sizeof(int), 1, exclusao);
                        fseek(r, -tamanhoEmpregado(), SEEK_CUR);    //voltando um reg
                        salva_empreg(prox, r);
                        free(e);    free(prox);
                        return 1;
                    }
                    else
                        e = prox;
                    // Não achamos nessa rodada do l
                }
            }
        }
        free(e);    free(prox);
        return 0;
    }
    else{
        printf("Não é possível ler arquivo de registros.\n");
    }
    free(e);
    free(prox);
    return 0;
}

/*int busca_por_cod(FILE *hash, FILE* regts, int cod, int tam, int l){ //retorna o endereço do arquivo de registros
	int end_atual;
	int chave = hash(cod, tam, l);
	Empregado* emp;
    FILE* r = fopen("r.dat", "w+b");

	fseek(hash, chave*sizeof(int), SEEK_SET);
	fread(&end_atual, sizeof(int), 1, hash);
	fseek(regts, end_atual*tamanhoEmpregado(), SEEK_SET);
	emp = le_empreg(regts);
	if(emp->cod == cod)
		return end_atual;
	else{
		while(emp->prox != -1){
			fseek(regts, emp->prox*tamanhoEmpregado(), SEEK_SET);
			end_atual = emp->prox;
			emp = le_empreg(r);
			if(emp->cod == cod)
				return end_atual;
		}
	}

	return -1;
}*/

int main(){
	int tamHash = 2, l = 0, qtd_registros = 0, p = 0, aux = -1;
	FILE *hash = fopen("hash.dat","w+b");
	FILE *regts = fopen("registros.dat","w+b");
	FILE *excl = fopen("exclusao.dat","w+b");
	Empregado *emp[3];

	initHash(hash, tamHash);
	fwrite(&aux, sizeof(int), 1, excl);
    
    emp[0] = criarEmpregado("x", 18, 100);
    emp[1] = criarEmpregado("y", 17, 100);
    emp[2] = criarEmpregado("z", 16, 100);
    imprime_empreg(emp[0]);
    imprime_empreg(emp[1]);
    imprime_empreg(emp[2]);
    
    printf("\n HASH: \n");
    imprimeHash(hash);
    
	inserirHash(hash, regts, excl, emp[0], tamHash, &p, l, &qtd_registros);
	inserirHash(hash, regts, excl, emp[1], tamHash, &p, l, &qtd_registros);
	inserirHash(hash, regts, excl, emp[2], tamHash, &p, l, &qtd_registros);
    printf(" Nova Hash: \n");
	//imprimeHash(hash);
    
    printf("  Última pos :\n");
    fseek(regts, 0, SEEK_END);
    fseek(regts, -tamanhoEmpregado(), SEEK_CUR);
    imprime_empreg(le_empreg(regts));
    
    //printf("Arq r:\n");
    //imprime_reg(regts);
    //printf(" Fim arq r\n");
	//percorrendo_lista(hash, regts, 1);

    printf("Excluindo reg na pos 1 (cod = 2) : %d\n", excluirHash(hash, regts, excl, 1, tamHash, &p, l, &qtd_registros));
    
    imprimeHash(hash);
    
    fclose(hash);    fclose(regts);    fclose(excl);
    
	return 0;
}
