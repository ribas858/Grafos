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

#define limit 40

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

typedef struct filaStruct{
	long int codicccgigo;
	struct filaStruct *proximaA;
}fila;

typedef struct pilhaStruct{
	long int codicccg;
	struct pilhaStruct *pilhaproximaA;
}pilha;

no *inicioG;
fila *filaE;
pilha *maximaPilha, *PLatual;
int maxvalor , ATvalor, maxATvalor = 0;

/*Simples interface para usuário*/
int Menu (){
	bool erro = true;
	int type;


	printf("\t 1 - Grafo (lista de adjacencias).                                   \n");
	printf("\t 2 - Ordenação Topologica.                                           \n");
	printf("\t 3 - Caminho Critico								                   \n");
	printf("\t 4 - Exit.                                                           \n");
	do{
		printf("\n\t\t>> Escolha uma.<<: ");
		scanf("%d", &type);
		if(type < 0 || type > 4){
			erro = false;
			printf("Tente de novo!!\n");
		}
		else
			erro = true;
	}while(!erro);
	return type;
}

/*Funções basicas para fila e pilha*/
/*A função inicializa um ponteiro para a pilha*/
void Ipinlha(pilha **ePilha){
	*ePilha = NULL;
}

/*A função EmptyPI retorna "falso ou verdadeiro" para a verificação se uma
** pilha esta ou não vazia*/
int EmptyPI (pilha *ePilha){
	return (ePilha == NULL);
}

/*A função PilhaTop retorna o elemento que se encontra no topo da pilha*/
long int PilhaTop (pilha *ePilha){
	return ePilha->codicccg;
}

/*A função InsertPilha recebe um elemento e o ponteiro para a pilha, insere o elemento
** e posiciona o ponteiro do topo da pilha para o ultimo elemento inserido na pilha.*/
void InsertPilha (pilha **ePilha, long int codicccg){
	pilha *pPilha;
	pPilha = malloc (sizeof(pilha));
	pPilha->codicccg = codicccg;
	pPilha->pilhaproximaA = *ePilha;
	*ePilha = pPilha;
}

/*A função tiraPilha remove o elemento no topo da pilha e reposiciona o ponteiro para o
** proximaAimo elemento da pilha, recebe um ponteiro para a pilha como paramentro.*/
int tiraPilha (pilha **ePilha){
	pilha *pPilha;
	long int codicccg;
	pPilha = *ePilha;
	*ePilha = pPilha->pilhaproximaA;
	codicccg = pPilha->codicccg;
	free(pPilha);
	return codicccg;
}

/*A função Ifila é chamada para inicializar um ponteiro para a fila*/
void Ifila (fila **filaE){
	*filaE = NULL;
}

/*A função Emptyfila retorna "falso ou verdadeiro" para
**verificação da fila vazia ou não respectivamente.*/
int Emptyfila (fila *filaE){
	return (filaE == NULL);
}

/*A função frentFILA retorna o elemento que está sempre na frente da fila*/
long int frentFILA (fila *filaE){
	return filaE->codicccgigo;
}
/*A função InsereFILA insere um novo elemento no final da
** fila sempre*/
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

/*A função cutFila remove sempre o primeiro elemento
** da fila, reposicionando o ponteiro para o proximaAimo elemento
** da fila e retorna o elemento removido da fila*/
int cutFila (fila **filaE){
	fila *pFila;
	long int codicccg;

	pFila = *filaE;
	*filaE = pFila->proximaA;
	codicccg = pFila->codicccgigo;
	free(pFila);
	return codicccg;
}

/**
** FUNÇÃO PARA CONSTRUÇAO DO GRAFO DIRECIONADO USANDO LISTAS LIGADA A LISTAS, COM ARCOS VALORADOS **
** E COM NÓS E ARCOS COM CAMPOS BOOLEANOS (zero ou um). A FUNÇÃO LÊ DO ARQUIVO E GERA UM GRAFO A PARTIR DOS   **
** DADOS LIDOS NO ARQUIVO, A CONSTRUÇÃO OCORRE EM DUAS INSTÂNCIAS DA LEITURA DO ARQUIVO, NA PRIMEIRA É
** CONSTRUIDO UMA LISTA COM TODOS OS ELEMENTOS (Vértices) E NA SEGUNDA AS SUAS DEPENDÊNCIAS SÃO GERADAS.
**/
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

/*Lê o grafo construído visitando cada nó e mostrando todos os seus
** adjacentes*/
void graforead (no *inicioG){
	no *pNo1;
	arco *pArc1 = NULL;

	if(inicioG == NULL)
		printf("Grafo vazio\n");
	else{
		printf("\t\tGrafo Direcionado Acíclico.\n\n");
		pNo1 = inicioG;
		while(pNo1 != NULL){
			printf("\n%ld: %s %d  %.1f", pNo1->codicccg, pNo1->namek, pNo1->Cr, pNo1->pesoPessoal);
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

/*A função somaGrauada recebe o ponteiro do inicio do
** grafo e conta o grau de entrada em todos os nós e
** armazena o valor de entradas nos respectivos nós no campo preReq
** e preReqAux que sera used para ordenação topologica sem perder o
** numero de requisitos da disciplina original*/
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

/*A função Otopologica recebe um ponteiro para o grafo e um ponteiro para uma fila
** executa o pseudo código visto na aula (Kahn) para ordenação topologica de um DAG
** a função usa fila para realizar a ordenação, o grafo é percorrido e os elementos com grau entr. zero
** são inseridos na fila, para cada elemento retirado da fila, contador de nós visitados é incrementado
** e o grau entr. dos vizinhos do nó são decrementado em 1, os vizinhos com graus entr. zero são incrementados
** a fila e o processo segue até a fila ficar vazia. O número de nós visitados são comparadados com o número
** de vértices no grafo para a verificação da possibilidade da ordenação.*/
void Otopologica(no *inicioG, fila **filaE){
	fila *pFila;
	no *pNo;
	arco *pArc;
	long int codicccgRetirado;
	int noVisitado = 0, nNo = 0;

	Ifila(&pFila);
	pNo = inicioG;

	/*Insere todos os nós com grau de entrada zero na fila*/
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

/*A função showOtopologica recebe um ponteiro para o grafo e outro para uma fila
**com os elementos ordenado do grafo e impreme os elementos e os graus dos vértices entrantes.
** O grafo ordenado não é apresentado de forma sequencial, mas sim em coluna onde a primeira linha
** dos elementos do grafico é o primeiro elemento na sequencia da ordenação, o segundo elemento da coluna
** é o segundo elemento na sequencia da ordenação, e assim por diante, considerando ordenação da esquerda
** para direita.(-_-)*/
void showOtopologica (no *inicioG, fila **filaE){
	fila *pFila;
	no *pGrafo;

	pGrafo = inicioG;
	if(Emptyfila(*filaE))
		printf("Fila vazia.\n");
	else{
		printf("\t\t\n\nOrdenação topologica.\n");
		Ifila(&pFila);
		while(!Emptyfila(*filaE)){
			while(frentFILA(*filaE) != pGrafo->codicccg)
				pGrafo = pGrafo->noPx;
			printf("%ld:%s requisito necessário -> (%d)\n", frentFILA(*filaE), pGrafo->namek, pGrafo->preReq);
			InsereFILA(&pFila, cutFila(filaE));
			pGrafo = inicioG;
		}
		while(!Emptyfila(pFila))
			InsereFILA(filaE, cutFila(&pFila));
	}
}

/**A função AchaCaminhoCritico determina os caminhos criticos para cada bifurcação a partir de um dado nó.
** Esta função recebe o ponteiro para um dado nó do grafo e a partir desta busca o maior caminho critico
** para este no passado e guarda o caminho critico descoberto em uma pilha. Para cada caminho achado, os valores
** são comparados e então salvos em maxvalor.*/
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

/** A função calculaCCritico é uma função auxiliar para determinação do caminho critico em um grafo.
** Recebe uma fila com os elementos da ordenação topologica e um ponteiro para o grafo. Ela retira cada elemento
** da fila e passa esse elemento para função AchaCaminhoCritico como ponteiro no grafo para que esta ache um caminho
** critico a partir deste nó. Mostra o caminho critico achado e o seu valor após passar todos os elementos da
** fila para verificação do caminho critico. Para cada maior caminho recebido, verifica-se o maior com os já recebido
** e então mostra o maior entre eles.*/
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
			printf("Erro na escolha de opção.");
			break;
	}
	return true;
}

void mostraCurso (){
	printf("\n\n\t\t===========================================\n");
	printf("\t\t=Ciência da Computação ((( Bacharel ))).=\n");
	printf("\t\t===========================================\n\n\n");
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
