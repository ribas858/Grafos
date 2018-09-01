/* 	Universidade de Brasilia
	Instituto de Ciencias Exatas
	Departamento de Ciencia da Computacao
	Teoria e Aplicação de Grafos - 1/2018
	Alunos: Diego Antonio Barbosa cardoso -16/0005116 , Lucas Junior Ribas - 16/0052289
	Versão do compilador:g++ (Ubuntu 7.2.0-8ubuntu3.2) 7.2.0
	Descricao: Este progama Cria um grafo usando lista de adjacencia utilizando como entrada o arquivo "karate.gml"
	ordena em ordem decrescente o vertice com maior grau e utiliza o algoritmo de Bron-Kerbosch para encontrar os cliques
	maximais
	Sites utilizados como exemplo : https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
	https://stackoverflow.com/questions/43759341/bron-kerbosch-algorithm-in-c?answertab=oldest#tab-top,
	https://www.geeksforgeeks.org/graph-implementation-using-stl-for-competitive-programming-set-1-dfs-of-unweighted-and-undirected/
	par executar utilize  g++ projet1.cpp -o projeto1 e depois utlize ./projeto1 lembrando que para o funcionamento adequado
	do progama o arquivo "karate.gml" precisa estar no mesmo diretorio que ele.
	 */

#include <bits/stdc++.h>
#include <iostream>
using namespace std;

vector<vector<int>> Maximal; // Esse vetor de vetor guarda os cliques maximais encontrados pelo algoritmo de Bron-Kerbosch.

// Essa funcao entra no arquivo "karate.gml" e cria uma lista de adjacencia com base no seu conteudo.
void CriaListaAdj(vector<int>  grafo[]){
	ifstream file;
	file.open("karate.gml", ios::in);
	string linha,sub;
	int s,t,j,tam;
	int i = 0;
	while( getline(file,linha)){
		tam = linha.length();
		if (tam > 4){
			sub = linha.substr(4,6);
		}
		if (sub == "source"){
			i++;
			s = 0;
			for ( j = 0 ;j != tam ; j++){
				if ((linha[j] >= 48) && (linha[j]) <= 57 ){
					if (s == 0) {
						s = (linha[j] - 48 );
					}
					else{
						s = (s*10) + (linha[j] - 48 );
					}
				}
			}
		}
		else if (sub == "target"){
				i++;
				t = 0;
			for ( j = 0 ; j != tam ; j++){
				if ((linha[j] >= 48) && (linha[j]) <= 57 ){
					if (t == 0) {
						t = (linha[j] - 48 );
					}
					else{
						t = (t*10) + (linha[j] - 48 );
					}
				}
			}
		}
		sub = "0";
		if (i == 2){
			i = 0;
			grafo[s].push_back(t);
			grafo[t].push_back(s);
		}
	}
	file.close();
}
// Essa funcao imprime na tela as listas de adjacencia em ordem decrescente de grau do vertice.
void ImprimeVerticeDecrescente(vector<int> grafo[], int vert){
	vector<pair<int,int>> dueto;
	int z = 0;
	int grau=0;
	for (int i = 1; i < vert; i++){
        z = grafo[i].size();
        dueto.push_back(pair<int,int>(i,z));
        z = 0;
    }
	sort(dueto.begin(), dueto.end(), [](auto &a, auto &b) {
    	return a.second > b.second;
	});
 	cout << endl<< "Vertices de MAIOR GRAU em ordem DECRESCENTE:" <<endl;
	for (int w = 0; w < vert -1; ++w){
        cout << "\n Vertice id: "<< dueto[w].first;

        for (int j : grafo[dueto[w].first]){
           //cout << " -> " << j;
					 grau++;
        }
				cout << "\n grau: " << grau+1;
				grau=0;
        cout << endl;
    }
}
// Impletação do algoritmo de Bron-Kerbosch para encontrar os cliques maximais
void BronKerbosch(vector<int> grafo[], vector<int> R , vector<int> P , vector<int> X){
	if(P.empty() && X.empty()){
		Maximal.push_back(R);
	}
	vector<int> P2 = P;
	for(int i : P2){
		vector<int> intersecaoP = {}, intersecaoX = {};
		for(int j : grafo[i]){
			for(int k : P){
				if(j == k){
					intersecaoP.push_back(j);
				}
			}
			for(int l : X){
				if(j == l){
					intersecaoX.push_back(j);
				}
			}
		}
		vector<int> R2 = R;
		R2.push_back(i);
		BronKerbosch(grafo,R2,intersecaoP,intersecaoX);
		P.erase(remove(P.begin(),P.end(),i),P.end());
		X.push_back(i);
	}
}
// Imprime na tela o clique maximal e um clique maximo diferente dele.
void ImprimeMaximal(vector<int> grafo[]){
	sort(Maximal.begin(), Maximal.end(), [](auto &a, auto &b) {
    	return a.size() > b.size();
	});
	cout << endl;
	cout << "Cliques Maximais: "<< endl<< endl;
	for (int j=0;j<35;j++){
		cout << "Clique Maximal: "<< endl;
		int h=0;
		for(int i : Maximal[j]) {
			cout << i << " ";
			h++;
			}
			cout << endl <<"numero de vertices:" << h << endl << endl;
	}
	cout << endl << endl;
}
int main(){
	int op=0;
	int vert = 35; // quantidade de vertices do grafo.
	vector<int> grafo[vert]; // um vetor de vetor que será usado como lista de adjacencia.
	vector<int> R,P,X; // vetores auxiliares para o algoritmo de BronKerbosch.
	CriaListaAdj(grafo);
	for (int i=1; i < vert; i++)
			P.push_back(i);

	BronKerbosch(grafo,R,P,X);

	cout << "Digite uma opção:" << endl;
	cout << "(1) o vértice, e seu respectivo grau (para todos os vértices)" << endl;
	cout << "(2) todos os cliques maximais (indicando o número de vértices e quais)" << endl;
	cout << "(3) O Coeficiente de Aglomeração de cada vértice" << endl;
	cout << "(4) O Coeficiente médio de Aglomeração do Grafo." << endl;
	cin  >> op;
	cin.ignore();
	switch (op) {
		case 1:
				ImprimeVerticeDecrescente(grafo,vert);
			break;
		case 2:
				ImprimeMaximal(grafo);
			break;
		case 3:
			break;
		case 4:
			break;
	}
	return 0;
}