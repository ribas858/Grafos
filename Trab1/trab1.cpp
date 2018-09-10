/* 	Instituto de Ciencias Exatas
	Departamento de Ciencia da Computacao
	Universidade de Brasilia
	Teoria e Aplicação de Grafos - TAG - 2/2018
	Alunos: 
	-- Lucas Junior Ribas - 16/0052289
	-- Marcelo Giordano Martins Costa de Oliveira - 12/0037301
	Versão do compilador: g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609
	Descricao: O programa contido neste arquivo gera um grafo nao direcionado usando lista de adjacencia, utilizando como entrada o arquivo "karate.gml" e o
	ordena em ordem decrescente, ou seja, do vertice com maior grau para o menor e tambem utiliza o algoritmo de Bron-Kerbosch para localizar os cliques
	maximais.

	Para que a execução do algoritmo ocorra corretamente é necessário que o comando "-std=c++14" sem aspas seja incluido na linha de compilação normal do C++,
	 ou seja, para executar corretamente digite extamente assim "g++ -std=c++14 trab1.cpp -o t1" sem aspas, caso haja algum problema nos contacte.
	 */

#include <bits/stdc++.h>
#include <iostream>
using namespace std;

vector<vector<int>> Maximal; // Cliques maximais localizados pelo algoritmo de Bron-Kerbosch.

// Parser para o "karate.gml". Obs: Não foi utilizado o parser fornecido.
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
// Imprime em ordem decrescente de grau do vertice.
void ImprimeVerticeDecrescente(vector<int> grafo[], int vert){
		vector<pair<int,int>> dueto;
		int zmanho = 0;
		for (int i = 1; i < vert; i++){
			zmanho = grafo[i].size();
			dueto.push_back(pair<int,int>(i,zmanho));
			zmanho = 0;
		}
		sort(dueto.begin(), dueto.end(), [](auto &a, auto &b) {
			return a.second > b.second;
		});
		cout << endl<< "Vertices de MAIOR GRAU em ordem DECRESCENTE:" <<endl;
		for (int w = 0; w < vert -1; ++w){
			cout << "\n Vertice id: "<< dueto[w].first;


			for (int j : grafo[dueto[w].first]){
			cout << " -> " << j;
						
			}
					//cout << "\n grau: " << grau+1;
					cout << "\n Grau: " << dueto[w].second;
					
			cout << endl;
		}
	}
	// Calcula o coeficiente de aglomeração do no e o medio, opção x==1 CoefNO, opção x==2 CoefMedio
	void CoeficienteAglomeracao ( vector<int> grafo[], int vert, int x){
	if (x == 1){
		vector<pair<int,int>> pares;
		int tam = 0;
		for (int i = 1; i < vert; i++){
			tam = grafo[i].size();
			pares.push_back(pair<int,int>(i,tam));
			tam = 0;
		}
		for (int j = 0; j < vert -1 ; j++){
			cout << "\n  NO: " << pares[j].first << " --";
			cout << " Vizinhos: " << pares[j].second;
			
			int trigPossiveis = pares[j].second * (pares[j].second-1);
			trigPossiveis = trigPossiveis/2;
			cout << "\nTriangulos Possiveis: " << trigPossiveis << endl;
			// k = 1 ADJ de B
			int t = 0;
			for (int k : grafo[pares[j].first]){
				//cout << "\n -> " << k;
				t = 0;
				// b = 1 ADJ de D
				for (int b : grafo[k]) {
					// c = 1 ADJ de B
					for (int c : grafo[pares[j].first]){
						//cout << " b: " << b;
						if ( b == c){
							t++;
							//cout << "\n " << k;
							//cout << " <-> " << c << endl;
						}
					}
				}
			}
			cout << "Triangulos Existentes: " << t << endl;
			cout << "COEF de Aglomeração: " << t << "/" << trigPossiveis << endl;
			cout << endl;
		}
	}else if (x == 2){
		vector<pair<int,int>> pares;
		int tam = 0;
		for (int i = 1; i < vert; i++){
			tam = grafo[i].size();
			pares.push_back(pair<int,int>(i,tam));
			tam = 0;
		}
		float media = 0;
		for (int j = 0; j < vert -1 ; j++){
			int trigPossiveis = pares[j].second * (pares[j].second-1);
			trigPossiveis = trigPossiveis/2;
			int t = 0;
			for (int k : grafo[pares[j].first]){
				t = 0;
				for (int b : grafo[k]) {
					for (int c : grafo[pares[j].first]){
						if ( b == c){
							t++;
						}
					}
				}
			}
			float div;
			if (trigPossiveis != 0){
				div = (float)t/(float)trigPossiveis;
			}else{
				div = 0;
			}
			media = media + div;
		}
		float CoefMedio;
		CoefMedio = 1.0/34.0;
		CoefMedio = CoefMedio * media;
		cout << "\nCoeficiente de Aglomeração Medio do GRAFO: "<<CoefMedio << endl << endl;
	}
	// for (int i = 1; i < vert ; i++){
	// 	cout << "\n no: " << i;
	// 	for (int j : grafo[i]){
	// 		cout << "  " << j;
	// 	}
	// }

}
// Bron-Kerbosch
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
	int max = Maximal.size();
	for (int j=0;j<max;j++){
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
	int vert = 35; // quantidade de vertices+1 do grafo.
	vector<int> grafo[vert]; // um vetor de vector que será usado como lista de adjacencia.
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
				CoeficienteAglomeracao(grafo,vert, 1);
			break;
		case 4:
				CoeficienteAglomeracao(grafo,vert, 2);
			break;
	}
	return 0;
}
