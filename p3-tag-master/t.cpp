/* 	Universidade de Brasilia
	Instituto de Ciencias Exatas
	Departamento de Ciencia da Computacao
	Teoria e Aplicação de Grafos - 1/2018
	Alunos: Diego Antonio Barbosa cardoso -16/0005116 , Lucas Junior Ribas - 16/0052289
	Versão do compilador:g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609
	Descricao: Este progama faz o maior emparelhamento estavel entre professores e escolas descrito no arquivo de entrada
	Sites utilizados como referencia :
    https://en.wikipedia.org/wiki/Stable_marriage_problem
    https://www.geeksforgeeks.org/stable-marriage-problem/
	Para executar utilize  g++ -std=c++11 t.cpp -o t e depois utlize ./t lembrando que para o funcionamento adequado
	do progama o arquivo : entradaProj3TAG.txt  deve estar no mesmo diretorio do arquvio t.cpp
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
 vector<pair<string,string>> casamentos;
class Professor{
  public:
    string name;
    int hab;
    bool casado = false;
    vector<string> escolas;

    Professor(string name, int hab, vector<string> escolas);
};

Professor::Professor(string name, int hab, vector<string> escolas){
    this->name = name;
    this->hab = hab;
    this->escolas = escolas;
}


class Escola{
  public:
    string name;
    int hab;
    int vagas;

    Escola(string name, int hab, int vagas);
};

Escola::Escola(string name, int hab, int vagas){
    this->name = name;
    this->hab = hab;
    this->vagas = vagas;
}


void criaListaPref(vector<Professor>& prof, vector<Escola>& esc){
    ifstream file;
    string linha;
    string sub = "";
    string name,comp;
    int hab;
    vector<string> escolas;
    int vagas;
    file.open("entradaProj3TAG.txt", ios::in);
    int flag = 0;
    int spacecount = 0;
    while(getline(file,linha)){
        if(linha.length()>35){
            if (linha == "//escolas e preferencias em termos de habilitacoes"){
                flag = 1;
            }
        }
        else{
            if((flag == 0)){
                for(int j = 0; j != linha.length() ;j++){
                    if((linha[j] != ',') && (linha[j] != '(') && (linha[j]) != ':' ){
                        if ((linha[j] != ' ') && (linha[j] != ')')){
                            sub = sub + linha[j];
                        }
                        else{
                            spacecount++;
                            if(spacecount == 1){
                                name = sub;
                            }
                            else if(spacecount == 2){
                                hab = stoi(sub);
                            }
                            else{
                                escolas.push_back(sub);
                            }
                            sub = "";
                        }
                    }
                }
                Professor p(name, hab, escolas);
                prof.push_back(p);
                escolas.erase(escolas.begin(),escolas.end());
                spacecount = 0;
                sub = "";
            }
            else{
                 for(int j = 0; j != linha.length() ;j++){
                    if((linha[j] != '(') && (linha[j] != ':')){
                        if ((linha[j] != ')')){
                            sub = sub + linha[j];
                        }
                        else{
                            spacecount++;
                            if(spacecount == 1){
                               name = sub;
                            }
                            else if(spacecount == 2){
                                hab = stoi(sub);
                            }
                            else{
                                vagas = stoi(sub);
                            }
                            sub = "";
                        }
                    }
                }
                Escola e(name,hab,vagas);
                esc.push_back(e);
                spacecount = 0;
                sub = "";
            }
        }
    }
}

int somaVagas(vector<Escola> esc){
    int soma= 0 ;
    for (int i = 0 ; i != esc.size(); i++){
        soma = soma + esc[i].vagas;
    }
    return soma;
}

void galeShapley(vector<Professor> prof, vector<Escola> esc){
    int i = somaVagas(esc);
    int j = 0;
    int pos =0;
    string sub;
    string num = "";
    while( i != 0){
        while(j != 99){
            if(prof[j].casado == false){
                for(int k = 0 ; k != 5 ; k++){
                  sub = prof[j].escolas[k];
                  for(int l =0; l != sub.length();l++){
                      if ((sub[l] >= '0') && (sub[l] <= '9')){
                          num = num + sub[l];
                      }
                  }
                  pos = stoi(num);
                  num = "";
                  if((esc[pos].vagas != 0) && (esc[pos].hab == prof[j].hab)){
                      prof[j].casado = true;
                      esc[pos].vagas --;
                      casamentos.push_back(make_pair(prof[j].name, esc[pos].name));
                  }
                }
            }
            j++;
        }
        i--;
    }

}

void printCasamento(){
    cout << "Casamentos estaveis: " <<endl<<endl;
    cout << "habilitacao 1 - 2 vagas ---- 8  prof" << endl;
    cout << "habilitacao 2 - 50 vagas ---- 40 profs" <<endl;
    cout << "habilitacao 3 - 32 vagas ---- 17 profs" << endl << endl;

    for (int i = 0; i != casamentos.size(); i++){
        cout << casamentos[i].first << "----"<< casamentos[i].second << endl;
    }
}

int main(){
    vector<Professor> prof;
    vector<Escola> esc;
    criaListaPref(prof,esc);
    galeShapley(prof,esc);
    printCasamento();
}
