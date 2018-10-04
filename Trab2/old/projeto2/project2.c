/************************************************************************************************																					   **
** NOTA: O programa é compilado com: gcc -ansi -Wall										   **
** para executá-lo: ./exe 																	   **
** onde o exe é o executável gerado na compilação											   **
** O programa oferece uma interface com usuário, permitindo-lhe escolher a execução de uma 	   **
** determinada função entre(mostrar DAG, Ordenação topologica e o caminho critico)	           **
** --------------------------------------------------------------------------------------------**
** MEMBRO: Marcelo Giordano M.C. de Oliveira - 12/0037301 Kevin Masinda Mahema - 130058866			DISCIPLINA: TEORIA E APLICAÇÃO DE GRAFOS       **
** ]							TURMA: 2/2017						       **
**												PROFESSOR: DÍBIO				  	           **
*************************************************************************************************
** O código contem as seguintes funções:													   **
** void ConstroiGrafo (no **egInicio)														   **
** LerGrafo(no *gIni)																		   **
** void ComputaGrauEntr(no *gIni)															   **
** void OrdenacaoTopologica(no *gIni, fila **eFila)											   **
** void MostraOrdenacaoTopologica (no *gIni, fila **eFila)									   **
** void DeterminaCaminhoCritico (fila **eFilha, no *ginicio)								   **
** void CaminhoCritico (no *pNo)															   **
**																							   **
** e as funções auxiliares:																	   **
** bool CallFunc ()																			   **
** int EscolhaFuncao ()																		   **
** Cada função tem uma descrição da sua funcionalidade e implementação.						   **
************************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define Limite 40

typedef struct elementoArco {
	int usado, peso;
	struct elementoArco *proxArco;
	struct elementoNo *pAdj;
}arco;

typedef struct elementoNo {
	int usado, Cr, nPreReq, nPreReqAux;
	float fat;
	long int cod;
	char nome[Limite];
	struct elementoNo *proxNo;
	struct elementoArco *pAcessoArco;
	/*struct elementoArco *pAcessoAdj[Limite];*/
}no;

typedef struct structFila{
	long int codigo;
	struct structFila *prox;
}fila;

typedef struct structPilha{
	long int cod;
	struct structPilha *pilhaProx;
}pilha;

no *gIni;
fila *eFila;
pilha *pilhaMax, *pilhaAtual;
int valormax , valoratual, valormaxAtual = 0;

/*Simples interface para usuário*/
int EscolhaFuncao (){
	bool erro = true;
	int type;

	printf("\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("\t+ [1] - Mostrar grafo (lista de adjacencias).          +\n");
	printf("\t+ [2] - MOstrar Ordenação Topologica.                  +\n");
	printf("\t+ [3] - Mostrar caminho critico.                       +\n");
	printf("\t+ [4] - Sair.                                          +\n");
	printf("\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	do{
		printf("\n\t\t>> Escolha uma das opções acima.<<: ");
		scanf("%d", &type);
		if(type < 0 || type > 4){
			erro = false;
			printf("Tente novamente.\n");
		}
		else
			erro = true;
	}while(!erro);
	return type;
}

/*Funções basicas para fila e pilha*/
/*A função inicializa um ponteiro para a pilha*/
void InicializaPilha(pilha **ePilha){
	*ePilha = NULL;
}

/*A função PilhaVazia retorna "falso ou verdadeiro" para a verificação se uma
** pilha esta ou não vazia*/
int PilhaVazia (pilha *ePilha){
	return (ePilha == NULL);
}

/*A função TopoPilha retorna o elemento que se encontra no topo da pilha*/
long int TopoPilha (pilha *ePilha){
	return ePilha->cod;
}

/*A função InserePilha recebe um elemento e o ponteiro para a pilha, insere o elemento
** e posiciona o ponteiro do topo da pilha para o ultimo elemento inserido na pilha.*/
void InserePilha (pilha **ePilha, long int cod){
	pilha *pPilha;
	pPilha = malloc (sizeof(pilha));
	pPilha->cod = cod;
	pPilha->pilhaProx = *ePilha;
	*ePilha = pPilha;
}

/*A função RetiraPilha remove o elemento no topo da pilha e reposiciona o ponteiro para o
** proximo elemento da pilha, recebe um ponteiro para a pilha como paramentro.*/
int RetiraPilha (pilha **ePilha){
	pilha *pPilha;
	long int cod;
	pPilha = *ePilha;
	*ePilha = pPilha->pilhaProx;
	cod = pPilha->cod;
	free(pPilha);
	return cod;
}

/*A função InicializaFila é chamada para inicializar um ponteiro para a fila*/
void InicializaFila (fila **eFila){
	*eFila = NULL;
}

/*A função FilaVazia retorna "falso ou verdadeiro" para
**verificação da fila vazia ou não respectivamente.*/
int FilaVazia (fila *eFila){
	return (eFila == NULL);
}

/*A função FrenteFila retorna o elemento que está sempre na frente da fila*/
long int FrenteFila (fila *eFila){
	return eFila->codigo;
}
/*A função insereFila insere um novo elemento no final da
** fila sempre*/
void InsereFila (fila **eFila, long int cod){
	fila *pFila1, *pFila2;

	pFila1 = malloc (sizeof (fila));
	pFila1->codigo = cod;
	pFila1->prox = NULL;
	if (*eFila == NULL)
		*eFila = pFila1;
	else {
		pFila2 = *eFila;
		while(pFila2->prox != NULL)
			pFila2 = pFila2->prox;
		pFila2->prox = pFila1;
	}
}

/*A função RetiraFila remove sempre o primeiro elemento
** da fila, reposicionando o ponteiro para o proximo elemento
** da fila e retorna o elemento removido da fila*/
int RetiraFila (fila **eFila){
	fila *pFila;
	long int cod;

	pFila = *eFila;
	*eFila = pFila->prox;
	cod = pFila->codigo;
	free(pFila);
	return cod;
}

/**
** FUNÇÃO PARA CONSTRUÇAO DO GRAFO DIRECIONADO USANDO LISTAS LIGADA A LISTAS, COM ARCOS VALORADOS **
** E COM NÓS E ARCOS COM CAMPOS BOOLEANOS (zero ou um). A FUNÇÃO LÊ DO ARQUIVO E GERA UM GRAFO A PARTIR DOS   **
** DADOS LIDOS NO ARQUIVO, A CONSTRUÇÃO OCORRE EM DUAS INSTÂNCIAS DA LEITURA DO ARQUIVO, NA PRIMEIRA É
** CONSTRUIDO UMA LISTA COM TODOS OS ELEMENTOS (Vértices) E NA SEGUNDA AS SUAS DEPENDÊNCIAS SÃO GERADAS.
**/
void ConstroiGrafo (no **egInicio){
	FILE *arq;
	no *pNo1, *pNo2, *pReq;
	arco *pArc1, *pArc2;
	char str[40], nome[Limite], c = ' ';
	int crd, i = 0;
	float fat;
	long int cod, preReq;

	memset(nome, '\0', Limite);


	*egInicio = NULL;
	arq = fopen ("disciplinas.txt", "r");
	if (arq == NULL)
		printf("\nArquivo não pode ser aberto\n");
	else{
		while ((fscanf (arq, "%ld | %[^|] | %d | %f | %[^\n]", &cod, nome, &crd, &fat, str)) != EOF){
			pNo1 = malloc (sizeof (no));
			while (nome[i] != '\0'){
				pNo1->nome[i] = nome[i];
				i++;
			}
			i = 0;

			pNo1->cod = cod;
			pNo1->Cr = crd;
			pNo1->fat = fat;
			pNo1->nPreReq = 0;
			pNo1->nPreReqAux = 0;
			pNo1->usado = 0;
			pNo1->pAcessoArco = NULL;
			pNo1->proxNo = NULL;
			if (*egInicio == NULL)
				*egInicio = pNo1;
			else
				pNo2->proxNo = pNo1;
			pNo2 = pNo1;
		}
		fclose (arq);
		pNo1 = *egInicio;

		pReq = *egInicio;
		arq = fopen ("disciplinas.txt", "r");
		while ((fscanf (arq, "%ld | %[^|] | %d | %f | ", &cod, nome, &crd, &fat)) != EOF){
			while(c != '\n'){
				pArc1 = malloc (sizeof(arco));
				fscanf (arq, "%ld", &preReq);
				c = getc (arq);
				if(preReq != 0){
					pArc1->usado = 0;
					pArc1->peso = (crd*fat);

					pArc1->pAdj = *egInicio;
					while(pArc1->pAdj->cod != cod) /*PROCURA A DISCIPLINA*/
						pArc1->pAdj = pArc1->pAdj->proxNo;

					while(pReq->cod != preReq)	/*PROCURA O PRÉ REQUISITO*/
						pReq = pReq->proxNo;

					pArc1->proxArco = NULL;
					if(pReq->pAcessoArco == NULL){ /*LIGA O PRÉ REQUISITO AO ARCO DA DISCIPLINA*/
						pReq->pAcessoArco = pArc1;
					}else{
						pArc2 = pReq->pAcessoArco;
						while(pArc2->proxArco != NULL)
							pArc2 = pArc2->proxArco;
						pArc2->proxArco = pArc1;
					}
					pArc2 = pArc1;
				}else
					free(pArc1);
				pReq = *egInicio;
			}
			c = ' ';
			pNo1 = pNo1->proxNo;

		}
	}
	fclose (arq);
}

/*Lê o grafo construído visitando cada nó e mostrando todos os seus
** adjacentes*/
void LerGrafo (no *gIni){
	no *pNo1;
	arco *pArc1 = NULL;

	if(gIni == NULL)
		printf("Grafo vazio\n");
	else{
		printf("\t\tGrafo Direcionado Acíclico.\n\n");
		pNo1 = gIni;
		while(pNo1 != NULL){
			printf("\n[%ld: %s %d  %.1f]", pNo1->cod, pNo1->nome, pNo1->Cr, pNo1->fat);
			if(pNo1->pAcessoArco != NULL)
				pArc1 = pNo1->pAcessoArco;
			else
				printf("->ninguém.");
			while(pArc1 != NULL){
				printf("->%ld", pArc1->pAdj->cod);
				pArc1 = pArc1->proxArco;
			}
			pNo1 = pNo1->proxNo;
		}
		printf("\n");
	}
}

/*A função ComputaGrauEntrada recebe o ponteiro do inicio do
** grafo e conta o grau de entrada em todos os nós e
** armazena o valor de entradas nos respectivos nós no campo nPreReq
** e nPreReqAux que sera usado para ordenação topologica sem perder o
** numero de requisitos da disciplina original*/
void ComputaGrauEntr(no *gIni){
	no *pNo;
	arco *pArc;

	pNo = gIni;
	while(pNo != NULL){
		if(pNo->pAcessoArco != NULL){
			pArc = pNo->pAcessoArco;
			while(pArc != NULL){
				pArc->pAdj->nPreReq++;
				pArc->pAdj->nPreReqAux++;
				pArc = pArc->proxArco;
			}
		}
		pNo = pNo->proxNo;
	}
}

/*A função OrdenacaoTopologica recebe um ponteiro para o grafo e um ponteiro para uma fila
** executa o pseudo código visto na aula (Kahn) para ordenação topologica de um DAG
** a função usa fila para realizar a ordenação, o grafo é percorrido e os elementos com grau entr. zero
** são inseridos na fila, para cada elemento retirado da fila, contador de nós visitados é incrementado
** e o grau entr. dos vizinhos do nó são decrementado em 1, os vizinhos com graus entr. zero são incrementados
** a fila e o processo segue até a fila ficar vazia. O número de nós visitados são comparadados com o número
** de vértices no grafo para a verificação da possibilidade da ordenação.*/
void OrdenacaoTopologica(no *gIni, fila **eFila){
	fila *pFila;
	no *pNo;
	arco *pArc;
	long int codRetirado;
	int noVisitado = 0, nNo = 0;

	InicializaFila(&pFila);
	pNo = gIni;

	/*Insere todos os nós com grau de entrada zero na fila*/
	while(pNo != NULL){
		if(pNo->nPreReqAux == 0)
			InsereFila(eFila, pNo->cod);
		pNo = pNo->proxNo;
	}

	pNo = gIni;
	while(!FilaVazia(*eFila)){
		codRetirado = RetiraFila(eFila);
		InsereFila(&pFila, codRetirado);
		noVisitado++;

		while(pNo->cod != codRetirado)
			pNo = pNo->proxNo;

		if(pNo->pAcessoArco != NULL){
			pArc = pNo->pAcessoArco;
			while(pArc != NULL){
				pArc->pAdj->nPreReqAux--;
				if(pArc->pAdj->nPreReqAux == 0)
					InsereFila(eFila, pArc->pAdj->cod);
				pArc = pArc->proxArco;
			}
		}
		pNo = gIni;
	}
	pNo = gIni;
	while(pNo != NULL){
		pNo = pNo->proxNo;
		nNo++;
	}
	if(nNo != noVisitado)
		printf("A ordenação topologica não foi possivel.\n");
	while(!FilaVazia(pFila))
		InsereFila(eFila, RetiraFila(&pFila));
}

/*A função MostraOrdenacaoTopologica recebe um ponteiro para o grafo e outro para uma fila
**com os elementos ordenado do grafo e impreme os elementos e os graus dos vértices entrantes.
** O grafo ordenado não é apresentado de forma sequencial, mas sim em coluna onde a primeira linha
** dos elementos do grafico é o primeiro elemento na sequencia da ordenação, o segundo elemento da coluna
** é o segundo elemento na sequencia da ordenação, e assim por diante, considerando ordenação da esquerda
** para direita.(-_-)*/
void MostraOrdenacaoTopologica (no *gIni, fila **eFila){
	fila *pFila;
	no *pGrafo;

	pGrafo = gIni;
	if(FilaVazia(*eFila))
		printf("Fila vazia.\n");
	else{
		printf("\t\t\n\nOrdenação topologica.\n");
		InicializaFila(&pFila);
		while(!FilaVazia(*eFila)){
			while(FrenteFila(*eFila) != pGrafo->cod)
				pGrafo = pGrafo->proxNo;
			printf("%ld:%s requisito necessário -> (%d)\n", FrenteFila(*eFila), pGrafo->nome, pGrafo->nPreReq);
			InsereFila(&pFila, RetiraFila(eFila));
			pGrafo = gIni;
		}
		while(!FilaVazia(pFila))
			InsereFila(eFila, RetiraFila(&pFila));
	}
}

/**A função CaminhoCritico determina os caminhos criticos para cada bifurcação a partir de um dado nó.
** Esta função recebe o ponteiro para um dado nó do grafo e a partir desta busca o maior caminho critico
** para este no passado e guarda o caminho critico descoberto em uma pilha. Para cada caminho achado, os valores
** são comparados e então salvos em valormax.*/
void CaminhoCritico (no *pNo) {
  	arco *pArc;
  	long int cod;

  	InserePilha (&pilhaAtual, pNo->cod);

  	pArc = pNo->pAcessoArco;
  	pNo->usado = 1;
  	if(pArc != NULL){
	  	while (pArc != NULL) {
	  		if (!pArc->pAdj->usado) {
	        	valoratual = valoratual + pArc->peso;
	        	CaminhoCritico (pArc->pAdj);
	        	valoratual = valoratual - pArc->peso;
	    	}
	    	pArc = pArc->proxArco;
	  	}
		pNo->usado = 0;
	}else{
		if(valoratual > valormax){
			valormax = valoratual;
			while(!PilhaVazia(pilhaMax))
				RetiraPilha(&pilhaMax);
			while(!PilhaVazia(pilhaAtual)){
				cod = RetiraPilha(&pilhaAtual);
				InserePilha(&pilhaMax, cod);
			}
		}
	}
}

/** A função DeterminaCaminhoCritico é uma função auxiliar para determinação do caminho critico em um grafo.
** Recebe uma fila com os elementos da ordenação topologica e um ponteiro para o grafo. Ela retira cada elemento
** da fila e passa esse elemento para função CaminhoCritico como ponteiro no grafo para que esta ache um caminho
** critico a partir deste nó. Mostra o caminho critico achado e o seu valor após passar todos os elementos da
** fila para verificação do caminho critico. Para cada maior caminho recebido, verifica-se o maior com os já recebido
** e então mostra o maior entre eles.*/
void DeterminaCaminhoCritico (fila **eFila, no *ginicio) {
  long int partida, cod;
  no *pNo;
  pilha *paux1;
  fila *faux;

  InicializaFila(&faux);
  InicializaPilha(&paux1);
  pNo = ginicio;

  if(ginicio != NULL){

  	while(!FilaVazia(*eFila)){
  		partida = RetiraFila(eFila);
  		InsereFila(&faux, partida);

	  	while(partida != pNo->cod)
	    	pNo = pNo->proxNo;

	    if (pNo == NULL)
      		printf ("no nao encontrado\n");
    	else {
	      	InicializaPilha (&pilhaAtual);
	      	InicializaPilha (&pilhaMax);
	      	valoratual = 0;
	      	valormax = 0;
	      	CaminhoCritico (pNo);

	      	if(valormax > valormaxAtual){
	      		valormaxAtual = valormax;
	      		while(!PilhaVazia(paux1))
	      			RetiraPilha(&paux1);
	      		while(!PilhaVazia(pilhaMax))
	      			InserePilha(&paux1, RetiraPilha(&pilhaMax));
	      	}
  		}
  		pNo = ginicio;
  	}
  	while(!PilhaVazia(pilhaMax))
  		RetiraPilha(&pilhaMax);
  	while(!PilhaVazia(paux1))
  		InserePilha(&pilhaMax, RetiraPilha(&paux1));

  	if (PilhaVazia (pilhaMax))
	    printf ("nao ha caminho\n");
	else {
		pNo = ginicio;
		printf(">> Caminho critico: \n");
	    printf("-> Valor do caminho critico: %d \n", valormaxAtual);
	    InicializaPilha (&paux1);
	    while (!PilhaVazia (pilhaMax))
	        InserePilha (&paux1, RetiraPilha (&pilhaMax));
	    while (!PilhaVazia (paux1)){
	    	cod = RetiraPilha(&paux1);
	    	while(pNo->cod != cod)
	    		pNo = pNo->proxNo;
	        printf ("-> %2ld:%s\n", pNo->cod, pNo->nome);
	    	pNo = ginicio;
	    }
	 }
	 while(!FilaVazia(faux))
	 	InsereFila(eFila, RetiraFila(&faux));
  }
  else
  	printf("\nGrafo vazio.\n\n");
}

/*CallFunc chama a função escolhida pelo usuário. Retorna true a chamada que não seja para encerrar o programa que neste
** caso retorna false.*/
bool CallFunc (){
	int type;

	type = EscolhaFuncao();
	switch(type){
		case 1:
			LerGrafo(gIni);
			break;
		case 2:
			MostraOrdenacaoTopologica(gIni, &eFila);
			break;
		case 3:
			DeterminaCaminhoCritico(&eFila, gIni);
			break;
		case 4:
			return false;
		default:
			printf("Erro na escolha de opção.");
			break;
	}
	return true;
}

void printCurso (){
	printf("\n\n\t\t*******************************************\n");
	printf("\t\t* Curso: Ciência da Computação (Bacharel).*\n");
	printf("\t\t*******************************************\n\n\n");
}

int main (void){
	bool True = true;
	ConstroiGrafo (&gIni);
	ComputaGrauEntr (gIni);
	OrdenacaoTopologica(gIni, &eFila);
	printCurso();

	while(True)
		True = CallFunc();
	printf("\n\n");
	return 0;
}
