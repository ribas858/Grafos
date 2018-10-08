/*
@jeffvfa - Jefferson Viana Fonseca Abreu 130028959
@renanlr - Renan Lobato Rheinboldt
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NV 43
#define DEBUG printf("aqui"); getchar();
#define DEBUGU printf("aquiu");

//elemento da lista de adjacência, aresta
typedef struct are {
    struct are *prox;
    int peso;
    int removido;
    struct ver *amigo;
    char *matricula;
} amigo;

//vértice do grafo
typedef struct ver {
    char *nome;
    char *matricula;
    int numAmigos;
    amigo *amigos;
    int dificuldade;
} aluno;

//lista de vértices

typedef struct vert {
    aluno * elemento;
    struct vert *prox;
    int peso;
} listaVertice;

aluno grafo_materias[NV];
listaVertice * ordena = NULL;

//protótipos de funções
int preencherGrafo();

void strip(char *);

void ligarAmigos();

void imprimirPorGrau();

char **str_split(char *, const char);

void menu();

aluno *buscaVertice(char *);

int contaMaxAmigos();

aluno *maisAmigos();

void inserirVertice(aluno*, listaVertice**);

void inserirVerticeFinal(aluno*, listaVertice**);

listaVertice* retirarVertice(listaVertice**);

listaVertice* criarConjuntoVerticesIniciais(listaVertice*);

int semAmigos(aluno*);

void ordenacaoTopologica();

int tamanhoLista(listaVertice*);

void imprimeLista(listaVertice*);

void caminhoCritico();

int eInicial(aluno*,listaVertice*);

int pesoPai(aluno*);

aluno * verticePai(aluno*);

void printaGrafo();

//corpo de funções

int eInicial(aluno * materia, listaVertice * listaInicial){
  listaVertice * aux;
  aux = listaInicial;

  while (aux != NULL) {
    if(materia == aux->elemento)
      return 1;
    aux = aux->prox;
  }
  return 0;
}

aluno * verticePai(aluno * elem){
    int i, pai;
    aluno * paiDoElem;
    for(i=0; i<=NV; i++){
        //printf("tá procurando o pai de %s\n", elem->nome);

        amigo * aux = (grafo_materias[i].amigos);

        while(aux != NULL){

            if(aux->amigo == elem) {
                //printf("acou o pai é %s\n", grafo_materias[i].nome);
                paiDoElem = &grafo_materias[i];
                return paiDoElem;
            }
            aux = (amigo *) aux->prox;
        }
    }
    return NULL;
}

void printaGrafo() {
  int i;
  for(i=0; i<NV; i++){
      printf("\t%s\n", grafo_materias[i].nome);
      amigo * aux = (grafo_materias[i].amigos);
      amigo * aux2 = malloc(sizeof(amigo));

      while(aux->matricula != NULL){
        printf("\t\t%s\n", (aux->amigo)->nome);
        aux = aux->prox;
      }
  }
    printf("\n\n\t>>> Pressione uma tecla para voltar ao menu <<<\n\t");
    getchar();
    getchar();
}


int pesoPai(aluno * elem){
  int i, pai;
  for(i=0; i<=NV; i++){


      amigo * aux = (grafo_materias[i].amigos);

      while(aux->matricula != NULL){

          if(aux->amigo == elem) {
              return grafo_materias[i].dificuldade;
          }
          aux = (amigo *) aux->prox;
      }
  }
  return 0;
}

void caminhoCritico(){
  listaVertice * lista = NULL,
  *aux = NULL,
  *listaCritica = NULL;

  amigo * auxm;

  aluno * maisDif = NULL;

  int i, maisDificil = 0;

  if (ordena == NULL) {
    ordenacaoTopologica();
  }

  //printf("caminhoCritico\n");

  for (i = 0; i < NV; i++) {
    //printf("entrou\n");
    auxm = grafo_materias[i].amigos;
    //printf("\n%s", grafo_materias[i].nome);
    if (auxm == NULL)
      continue;

    while(auxm != NULL){
        //printf("removido : %d depois:", auxm-> removido);
        if(auxm-> removido != 0){

            auxm-> removido = 0;
        }
        //printf("%d\n", auxm-> removido);
        auxm = auxm->prox;
    }
  }

  lista = criarConjuntoVerticesIniciais(lista);

  aux = ordena;
  /*
  int xoom = tamanhoLista(lista);

  printf("\t\t\t\tcriou inicial de %d\n\n\n\n\n", xoom);

  imprimeLista(lista);
  */
  while (aux != NULL) {
    if(eInicial(aux->elemento,lista)){
      //printf("Entrou SE\n");
    }

    else{
      //printf("Entrou SEnão\n");
      int pai;
      pai = pesoPai(aux->elemento);
      (aux->elemento)->dificuldade += pai;
      aux->peso = (aux->elemento)->dificuldade;
    }
    //printf("\t\t%s tem peso : %d\n", (aux->elemento)->nome,(aux->elemento)->dificuldade);

    if ((aux->elemento)->dificuldade > maisDificil) {
      maisDificil = (aux->elemento)->dificuldade;
      maisDif = (aux->elemento);
    }

    aux = aux->prox;
  }

  printf("\t%s é o ciclo mais difícil com o peso %d\n\tCaminho:\n\n",maisDif->nome, maisDificil);

  while (maisDif != NULL) {
    /*printf("entrou wh\n");
    DEBUG
    printf("inseriu %s\n", maisDif->nome);*/
    inserirVertice(maisDif, &listaCritica);
    maisDif = verticePai(maisDif);
    //printf("%s é o pai\n", maisDif->nome);
  }

  imprimeLista(listaCritica);
}


void imprimeLista(listaVertice * lista){
    //printf("\tStartPrint\n");
    listaVertice * aux;
    //printf("%p : \n", aux);
    aux = lista;
    ///printf("%s -> ", (aux->elemento)->nome);
    int k = 0;
    printf("\t");
    while (aux != NULL) {
      if (k==3){
        printf("%s ->\n\t", (aux->elemento)->nome);
        k = 0;
      } else {
        printf("%s -> ", (aux->elemento)->nome);
        k++;
      }
      aux = aux->prox;
    }
    printf("\n\n\t>>> Pressione uma tecla para voltar ao menu <<<\n\t");
    getchar();
    getchar();
    //printf("\tEndPrint\n");
}

int tamanhoLista(listaVertice* lista){
  int i =0;
  listaVertice * aux;
  aux = lista;

  while (aux != NULL) {
    i++;
    //printf("%d\n", i);
    aux = aux->prox;
  }
  return i;
}

listaVertice* retirarVertice(listaVertice ** lista){
  if (*lista == NULL) {
    return NULL;
  }

  listaVertice *aux = NULL;

  aux = *lista;
  *lista = (*lista)->prox;

  aux->prox = NULL;

  return aux;
}

int semAmigos(aluno * elem){

    int i;
    for(i=0; i<=NV; i++){


        amigo * aux = (grafo_materias[i].amigos);/* printf("%s",(aux->amigo)->nome);*/
        while(aux != NULL){
            //printf("%s = %s ?",(aux->amigo)->nome, elem->nome);
            if(aux->amigo == elem && aux->removido == 0) {
                return 0;
            }
            aux = (amigo *) aux->prox;
        }
    }
    return 1;
}

/*
L ← Lista vazia que irá conter os elementos ordenados
S ← Conjunto de todos os nós sem arestas de entrada
enquanto S é não-vazio faça
    remova um nodo n de S
    insira n em L
    para cada nodo m com uma aresta e de n até m faça
        remova a aresta e do grafo
        se m não tem mais arestas de entrada então
            insira  m em S
se o grafo tem arestas então
    escrever mensagem de erro (grafo tem pelo menos um ciclo)
senão
    escrever mensagem  (ordenação topológica proposta: L)
*/
void ordenacaoTopologica(){
    listaVertice * aux = NULL,
    * inicio = NULL;
    inicio = criarConjuntoVerticesIniciais(inicio);

    int i = tamanhoLista(inicio);

    /*
    printf("DEVERIA IMPRIMIR A LISTA de tamanho %d\n",i);
    imprimeLista(inicio);
    */
    int visitados = 0;
    amigo * auxm;

    while(inicio != NULL && tamanhoLista(ordena) != NV){

        aux = retirarVertice(&inicio);
        if (aux == NULL)
          break;

        //printf("%s entrou na ordenação\n", (aux->elemento)->nome);
        inserirVerticeFinal(aux->elemento, &ordena);

        //imprimeLista(ordena);


        auxm = (aux->elemento)->amigos;

        if (auxm == NULL)
          break;

        while(auxm != NULL){
          //printf("entrou while\n.\n.\n");
            if(auxm-> removido != 1){
                //printf("removeu a aresta\n");
                auxm-> removido = 1;
            }
            if(semAmigos(auxm->amigo)){
                //printf("não tinha mais amigos colocou na lista\n");
                inserirVertice(auxm->amigo, &inicio);
            }

            auxm = auxm->prox;
        }
    }
    printf("\tOrdenacao Topologica: \n\n");
    imprimeLista(ordena);
}

void inserirVertice(aluno * elem, listaVertice ** list){
    //imprimeLista(*list);
    listaVertice *aux = NULL;
    aux = (listaVertice*) malloc(sizeof(listaVertice));
    aux->elemento = elem;
    //printf("automaticamente: %s\n", (aux->elemento)->nome);

    aux->prox = *list;
    *list = aux;
    //printf("vai imprimeLista\n");
}

void inserirVerticeFinal(aluno * elem, listaVertice ** list){
  //printf("\tStartPrint\n");
  listaVertice * aux, *novoVert;
  aux = *list;

  novoVert = (listaVertice*) malloc(sizeof(listaVertice));
  novoVert->elemento = elem;
  novoVert->prox = NULL;

  //printf("%s\n",(novoVert->elemento)->nome );

  if (aux == NULL) {
    *list = novoVert;
  }

  while (aux != NULL) {
    if (aux->prox == NULL) {
      aux->prox = novoVert;
      return;
    }
    aux = aux->prox;
  }
  //printf("\tEndPrint\n");
}

listaVertice* criarConjuntoVerticesIniciais(listaVertice* list){

    int i, j;
    for(i=0; i<NV; i++){
        //printf("%s\n",grafo_materias[i].nome);

        if(semAmigos(&grafo_materias[i])) {
            //printf("%s não tinha amigos e vai pra lista\n",grafo_materias[i].nome);
            inserirVerticeFinal(&grafo_materias[i],&list);
            }
    }

    //imprimeLista(list);
    return list;
}

aluno *maisAmigos() {
    int i, maisAmigos = 0;
    aluno *aluno = NULL;
    for (i = 0; i < NV; i++) {
        maisAmigos = grafo_materias[i].numAmigos > maisAmigos ? grafo_materias[i].numAmigos : maisAmigos;
        aluno = grafo_materias[i].numAmigos > maisAmigos ? &grafo_materias[i] : aluno;
    }
    return aluno;
}

//retorna a maior quantidade de amigos
int contaMaxAmigos() {
    int i, maisAmigos = 0;
    for (i = 0; i < NV; i++) {
        maisAmigos = (grafo_materias[i].numAmigos) > maisAmigos ? grafo_materias[i].numAmigos : maisAmigos;
    }
    return maisAmigos;
}

//função que busca um vértice através de sua matrícula
aluno *buscaVertice(char *target) {
    aluno *retorno = NULL;
    //varre a lista
    int i;
    for (i = 0; i < NV; i++) {
        if (strcmp((grafo_materias[i].matricula), target) == 0) {
            //se encontra o vértice retorna o endereço do vértice
            retorno = &grafo_materias[i];
            return retorno;
        }
    }
    //se não retorna um ponteiro nulo
    return retorno;
}

//invoca o menu
void menu() {
    int escape = 1, opt;

    do {
        system("clear||cls");
        printf("\t====================================================\n");
        printf("\t======================= Menu =======================\n");
        printf("\t====================================================\n");
        printf("\t==========Curso: CIÊNCIA DA COMPUTAÇÃO =============\n");
        printf("\t====================================================\n");
        printf("\t========= 1) Imprimir ordenação topológica =========\n");
        printf("\t========= 2) Imprimir o caminho crítico   ==========\n");
        printf("\t========= 3) Imprimir o grafo             ==========\n");
        printf("\t========= 4) Sair                         ==========\n");
        printf("\t====================================================\n");
        printf("\t====================================================\n");
        printf("\t====================================================\n\t>>>");
        scanf("%d", &opt);
        switch (opt) {
            case 1:
                system("clear||cls");
                ordenacaoTopologica();
                break;
            case 2:
                system("clear||cls");
                caminhoCritico();
                break;
            case 3:
                system("clear||cls");
                printaGrafo();
                break;
            case 4:
                system("clear||cls");
                escape = 0;
                break;
            default:
                break;
        }
    } while (escape);
}

int preencherGrafo() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **tokens;
    int i, j = 0, transforma;
    aluno *alvo;

    //abre-se o arquivo
    fp = fopen("materias_tag20171.txt", "r");
    if (fp == NULL) {
        return -1;
    }


    while ((read = getline(&line, &len, fp)) != -1) {
      //printf("%s\n", line);
        strip(line);
        // Separando linha por virgulas
        tokens = str_split(line, ',');

        if (tokens) {

            //printf("%s  ,  %s  ,   %s   ,  %s ...\n", *(tokens + 1),*(tokens + 2),*(tokens + 3),*(tokens + 4));
            //inicializando a lista de amigos como uma lista vazia
            grafo_materias[j].amigos = NULL;
            //inserindo nome do aluno no grafo
            grafo_materias[j].nome = *(tokens);
            //inserindo matricula do aluno
            grafo_materias[j].matricula = *(tokens + 1);
            //dificuldadeno vertice também
            transforma = (int) *(*(tokens + 2));
            transforma -= 48;
            grafo_materias[j].dificuldade = transforma;

            // inserindo matricula de amigos
            amigo *ultimoDaLista;

            grafo_materias[j].amigos = malloc(sizeof(amigo));
            ultimoDaLista = grafo_materias[j].amigos;

            ultimoDaLista->peso = transforma;

            ultimoDaLista->removido = (int) 0;

            for (i = 3; *(tokens + i); i++) {

                ultimoDaLista->matricula = *(tokens + i);
                ultimoDaLista->prox = malloc(sizeof(amigo));
                ultimoDaLista = (amigo *) ultimoDaLista->prox;

            }

            grafo_materias[j].numAmigos = i - 2;

            //printf("\n");
        }
        j++;
    }
    fclose(fp);
    ligarAmigos();
    return 0;
}

// Este método deve relacionar a lista de amigos com o seu respectivo nó
void ligarAmigos() {
    int k;
    amigo *elementoLista;
    for (k = 0; k < NV; k++) {
        elementoLista = grafo_materias[k].amigos;
        while (elementoLista->matricula != NULL) {
            elementoLista->amigo = buscaVertice(elementoLista->matricula);
            elementoLista = (amigo *)elementoLista->prox;
        }
    }
}
void strip(char *s) {
    char *p2 = s;
    while (*s != '\0') {
        if (*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main() {
  preencherGrafo();

  menu();

  return 0;
}
