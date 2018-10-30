#define limit 40
/*																					 
O programa é compilado com: gcc -o namek namek.c										   
executando no terminal: ./namek		

	Instituto de Ciencias Exatas
	Departamento de Ciencia da Computacao
	Universidade de Brasilia
	Teoria e Aplicação de Grafos - TAG - 2/2018
	Alunos: 
	-- Lucas Junior Ribas - 16/0052289
	-- Marcelo Giordano Martins Costa de Oliveira - 12/0037301
	Versão do compilador: g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609															   **
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void mostraCurso (){
	printf("\n\n\t\t===========================================\n");
	printf("\t\t=Ciência da Computação ((( Bacharel ))).=\n");
	printf("\t\t===========================================\n\n\n");
}

typedef struct pilhaStruct{
	long int codicccg;
	struct pilhaStruct *pilhaproximaA;
}pilha;
typedef struct filaStruct{
	long int codicccgigo;
	struct filaStruct *proximaA;
}fila;
typedef struct HEarco {
	int used, valoracaoGR;
	struct HEarco *pxarco;
	struct NoElement *pxAdjacente;
}arco;
typedef struct NoElement {
	int used, Cr, preReq, preReqAux;
	float pesoPessoal;
	long int codicccg;
	char namek[limit];
	struct NoElement *noPx;
	struct HEarco *ParcoAcess;
}no;
no *inicioG;


int maxvalor , ATvalor, maxATvalor = 0;
pilha *maximaPilha, *PLatual;
fila *filaE;

/*retira do topo da pilha*/
int tiraPilha (pilha **ePilha){
	pilha *pPilha;
	long int codicccg;
	pPilha = *ePilha;
	*ePilha = pPilha->pilhaproximaA;
	codicccg = pPilha->codicccg;
	free(pPilha);
	return codicccg;
}

/*ve se a pilha esta vazia*/
int EmptyPI (pilha *ePilha){
	return (ePilha == NULL);
}

/*inicia fila*/
void Ifila (fila **filaE){
	*filaE = NULL;
}

/*ve se a pilha esta vazia*/
int Emptyfila (fila *filaE){
	return (filaE == NULL);
}

/*retorna o primeiro elemento da fila*/
long int frentFILA (fila *filaE){
	return filaE->codicccgigo;
}
/*insere no final da fila*/
void InsereFILA (fila **filaE, long int codicccg){
	fila *pFila1, *pFila2;

	pFila1 = malloc (sizeof (fila));
	pFila1->codicccgigo = codicccg;
	pFila1->proximaA = NULL;
	if (*filaE == NULL)
		*filaE = pFila1;
	else {
		pFila2 = *filaE;
		while(pFila2->proximaA != NULL)
			pFila2 = pFila2->proximaA;
		pFila2->proximaA = pFila1;
	}
}

/*inicia pilha*/
void Ipinlha(pilha **ePilha){
	*ePilha = NULL;
}

/*insere no topo da pilha*/
void InsertPilha (pilha **ePilha, long int codicccg){
	pilha *pPilha;
	pPilha = malloc (sizeof(pilha));
	pPilha->codicccg = codicccg;
	pPilha->pilhaproximaA = *ePilha;
	*ePilha = pPilha;
}

/*remove o primeiro elemento da fila */
int cutFila (fila **filaE){
	fila *pFila;
	long int codicccg;

	pFila = *filaE;
	*filaE = pFila->proximaA;
	codicccg = pFila->codicccgigo;
	free(pFila);
	return codicccg;
}

/*retorna topo da pilha*/
long int PilhaTop (pilha *ePilha){
	return ePilha->codicccg;
}

/*cria o grafo usando lista ligada a lista*/
void CriaGrafo (no **einicioGcio){
	FILE *arq;
	no *pNo1, *pNo2, *pReq;
	arco *pArc1, *pArc2;
	char str[40], namek[limit], c = ' ';
	int crd, i = 0;
	float pesoPessoal;
	long int codicccg, preReq;
	memset(namek, '\0', limit);
	*einicioGcio = NULL;
	arq = fopen ("disciplinas.txt", "r");
	if (arq == NULL)
		printf("\nArquivo não pode ser aberto\n");
	else{
		while ((fscanf (arq, "%ld | %[^|] | %d | %f | %[^\n]", &codicccg, namek, &crd, &pesoPessoal, str)) != EOF){
			pNo1 = malloc (sizeof (no));
			while (namek[i] != '\0'){
				pNo1->namek[i] = namek[i];
				i++;
			}
			i = 0;
			pNo1->codicccg = codicccg;
			pNo1->Cr = crd;
			pNo1->pesoPessoal = pesoPessoal;
			pNo1->preReq = 0;
			pNo1->preReqAux = 0;
			pNo1->used = 0;
			pNo1->ParcoAcess = NULL;
			pNo1->noPx = NULL;
			if (*einicioGcio == NULL)
				*einicioGcio = pNo1;
			else
				pNo2->noPx = pNo1;
			pNo2 = pNo1;
		}
		fclose (arq);
		pNo1 = *einicioGcio;
		pReq = *einicioGcio;
		arq = fopen ("disciplinas.txt", "r");
		while ((fscanf (arq, "%ld | %[^|] | %d | %f | ", &codicccg, namek, &crd, &pesoPessoal)) != EOF){
			while(c != '\n'){
				pArc1 = malloc (sizeof(arco));
				fscanf (arq, "%ld", &preReq);
				c = getc (arq);
				if(preReq != 0){
					pArc1->used = 0;
					pArc1->valoracaoGR = (crd*pesoPessoal);

					pArc1->pxAdjacente = *einicioGcio;
					while(pArc1->pxAdjacente->codicccg != codicccg) /*PROCURA A DISCIPLINA*/
						pArc1->pxAdjacente = pArc1->pxAdjacente->noPx;

					while(pReq->codicccg != preReq)	/*PROCURA O PRÉ REQUISITO*/
						pReq = pReq->noPx;

					pArc1->pxarco = NULL;
					if(pReq->ParcoAcess == NULL){ /*LIGA O PRÉ REQUISITO AO ARCO DA DISCIPLINA*/
						pReq->ParcoAcess = pArc1;
					}else{
						pArc2 = pReq->ParcoAcess;
						while(pArc2->pxarco != NULL)
							pArc2 = pArc2->pxarco;
						pArc2->pxarco = pArc1;
					}
					pArc2 = pArc1;
				}else
					free(pArc1);
				pReq = *einicioGcio;
			}
			c = ' ';
			pNo1 = pNo1->noPx;
		}
	}
	fclose (arq);
}

/*Lê o grafo*/
void graforead (no *inicioG){
	no *pNo1;
	arco *pArc1 = NULL;

	if(inicioG == NULL)
		printf("Grafo vazio\n");
	else{
		printf("\t\tGrafo DIRECIONADO.\n\n");
		pNo1 = inicioG;
		while(pNo1 != NULL){
			printf("\nCODIGO DISCIPLINA:%ld<+> %s %d  %.1f", pNo1->codicccg, pNo1->namek, pNo1->Cr, pNo1->pesoPessoal);
			if(pNo1->ParcoAcess != NULL)
				pArc1 = pNo1->ParcoAcess;
			else
				printf("->nada.");
			while(pArc1 != NULL){
				printf("->%ld", pArc1->pxAdjacente->codicccg);
				pArc1 = pArc1->pxarco;
			}
			pNo1 = pNo1->noPx;
		}
		printf("\n");
	}
}

/*soma os graus dos nos de acordo com o pre-requisito*/
void somaGrau(no *inicioG){
	no *pNo;
	arco *pArc;

	pNo = inicioG;
	while(pNo != NULL){
		if(pNo->ParcoAcess != NULL){
			pArc = pNo->ParcoAcess;
			while(pArc != NULL){
				pArc->pxAdjacente->preReq++;
				pArc->pxAdjacente->preReqAux++;
				pArc = pArc->pxarco;
			}
		}
		pNo = pNo->noPx;
	}
}

/*Ordenaçao topologica.*/
void Otopologica(no *inicioG, fila **filaE){
	fila *pFila;
	no *pNo;
	arco *pArc;
	long int codicccgRetirado;
	int noVisitado = 0, nNo = 0;
	Ifila(&pFila);
	pNo = inicioG;
	while(pNo != NULL){
		if(pNo->preReqAux == 0)
			InsereFILA(filaE, pNo->codicccg);
		pNo = pNo->noPx;
	}
	pNo = inicioG;
	while(!Emptyfila(*filaE)){
		codicccgRetirado = cutFila(filaE);
		InsereFILA(&pFila, codicccgRetirado);
		noVisitado++;

		while(pNo->codicccg != codicccgRetirado)
			pNo = pNo->noPx;
		if(pNo->ParcoAcess != NULL){
			pArc = pNo->ParcoAcess;
			while(pArc != NULL){
				pArc->pxAdjacente->preReqAux--;
				if(pArc->pxAdjacente->preReqAux == 0)
					InsereFILA(filaE, pArc->pxAdjacente->codicccg);
				pArc = pArc->pxarco;
			}
		}
		pNo = inicioG;
	}
	pNo = inicioG;
	while(pNo != NULL){
		pNo = pNo->noPx;
		nNo++;
	}
	if(nNo != noVisitado)
		printf("A ordenação topologica não foi possivel.\n");
	while(!Emptyfila(pFila))
		InsereFILA(filaE, cutFila(&pFila));
}

/*mostra a ordenaçao topologica na tela*/
void showOtopologica (no *inicioG, fila **filaE){
	fila *pFila;
	no *pGrafo;
	pGrafo = inicioG;
	if(Emptyfila(*filaE))
		printf("FILA VAZIA.\n");
	else{
		printf("\t\t\n\n                ORDENACAO TOPOLOGICA.\n");
		Ifila(&pFila);
		while(!Emptyfila(*filaE)){
			while(frentFILA(*filaE) != pGrafo->codicccg)
				pGrafo = pGrafo->noPx;
			printf("CODIGO DISCIPLINA:%ld<+>%s === REQUISITO ==>> (%d)\n", frentFILA(*filaE), pGrafo->namek, pGrafo->preReq);
			InsereFILA(&pFila, cutFila(filaE));
			pGrafo = inicioG;
		}
		while(!Emptyfila(pFila))
			InsereFILA(filaE, cutFila(&pFila));
	}
}

/**nome auto-explicativo*/
void AchaCaminhoCritico (no *pNo) {
  	arco *pArc;
  	long int codicccg;
  	InsertPilha (&PLatual, pNo->codicccg);
  	pArc = pNo->ParcoAcess;
  	pNo->used = 1;
  	if(pArc != NULL){
	  	while (pArc != NULL) {
	  		if (!pArc->pxAdjacente->used) {
	        	ATvalor = ATvalor + pArc->valoracaoGR;
	        	AchaCaminhoCritico (pArc->pxAdjacente);
	        	ATvalor = ATvalor - pArc->valoracaoGR;
	    	}
	    	pArc = pArc->pxarco;
	  	}
		pNo->used = 0;
	}else{
		if(ATvalor > maxvalor){
			maxvalor = ATvalor;
			while(!EmptyPI(maximaPilha))
				tiraPilha(&maximaPilha);
			while(!EmptyPI(PLatual)){
				codicccg = tiraPilha(&PLatual);
				InsertPilha(&maximaPilha, codicccg);
			}
		}
	}
}

/** calcula caminho critico*/
void calculaCCritico (fila **filaE, no *inicioGcio) {
  long int partida, codicccg;
  no *pNo;
  pilha *paux1;
  fila *faux;
  Ifila(&faux);
  Ipinlha(&paux1);
  pNo = inicioGcio;
  if(inicioGcio != NULL){

  	while(!Emptyfila(*filaE)){
  		partida = cutFila(filaE);
  		InsereFILA(&faux, partida);
	  	while(partida != pNo->codicccg)
	    	pNo = pNo->noPx;
	    if (pNo == NULL)
      		printf ("no nao encontrado\n");
    	else {
	      	Ipinlha (&PLatual);
	      	Ipinlha (&maximaPilha);
	      	ATvalor = 0;
	      	maxvalor = 0;
	      	AchaCaminhoCritico (pNo);

	      	if(maxvalor > maxATvalor){
	      		maxATvalor = maxvalor;
	      		while(!EmptyPI(paux1))
	      			tiraPilha(&paux1);
	      		while(!EmptyPI(maximaPilha))
	      			InsertPilha(&paux1, tiraPilha(&maximaPilha));
	      	}
  		}
  		pNo = inicioGcio;
  	}
  	while(!EmptyPI(maximaPilha))
  		tiraPilha(&maximaPilha);
  	while(!EmptyPI(paux1))
  		InsertPilha(&maximaPilha, tiraPilha(&paux1));

  	if (EmptyPI (maximaPilha))
	    printf ("nao ha caminho\n");
	else {
		pNo = inicioGcio;
		printf(">> Caminho critico: \n");
	    printf("-> Valor do caminho critico: %d \n", maxATvalor);
	    Ipinlha (&paux1);
	    while (!EmptyPI (maximaPilha))
	        InsertPilha (&paux1, tiraPilha (&maximaPilha));
	    while (!EmptyPI (paux1)){
	    	codicccg = tiraPilha(&paux1);
	    	while(pNo->codicccg != codicccg)
	    		pNo = pNo->noPx;
	        printf ("-> %2ld:%s\n", pNo->codicccg, pNo->namek);
	    	pNo = inicioGcio;
	    }
	 }
	 while(!Emptyfila(faux))
	 	InsereFILA(filaE, cutFila(&faux));
  }
  else
  	printf("\nGrafo vazio.\n\n");
}
int Menu (){
	bool e = true;
	int t;
	printf("\t 1 - Grafo (lista de adjacencias).                                   \n");
	printf("\t 2 - Ordenação Topologica.                                           \n");
	printf("\t 3 - Caminho Critico								                   \n");
	printf("\t 4 - Exit.                                                           \n");
	do{
		printf("\n\t\t>> Escolha uma.<<: ");
		scanf("%d", &t);
		if(t < 0 || t > 4){
			e = false;
			printf("Tente de novo!!\n");
		}
		else
			e = true;
	}while(!e);
	return t;
}

bool DetFunc (){
	int type;

	type = Menu();
	switch(type){
		case 1:
			graforead(inicioG);
			break;
		case 2:
			showOtopologica(inicioG, &filaE);
			break;
		case 3:
			calculaCCritico(&filaE, inicioG);
			break;
		case 4:
			return false;
		default:
			printf("e na escolha de opção.");
			break;
	}
	return true;
}

int main (void){
	bool True = true;
	CriaGrafo (&inicioG);
	somaGrau (inicioG);
	Otopologica(inicioG, &filaE);
	mostraCurso();

	while(True)
		True = DetFunc();
	printf("\n\n");
	return 0;
}
