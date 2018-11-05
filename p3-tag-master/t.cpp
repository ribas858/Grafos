/* 	
	Teoria e Aplicação de Grafos - 2/2018
	
	Universidade de Brasilia

	Departamento de Ciencia da Computacao
	
	Alunos: 
		Lucas Junior Ribas - 16/0052289
		Marcelo Giordano Martins Costa de Oliveira - 12/0037301

	Execução  g++ -std=c++11 t.cpp -o t e depois ./t
	
	Deixe o arquivo entradaProj3TAG.txt no mesmo diretorio.
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
                      if (casamentos.size() < 70) {
                      	casamentos.push_back(make_pair(prof[j].name, esc[pos].name));
                      }
                  }
                }
            }
            j++;
        }
        i--;
    }

}

void printCasamento(){
	int j;
	j = 0;
    for (int i = 0; i != casamentos.size(); i++){
    	j++;
    	if (casamentos[i].first.size() == 2 && casamentos[i].second.size() == 2){
    		cout << casamentos[i].first << " ----"<< casamentos[i].second << "  ";
    	}else{
	    	if( casamentos[i].first.size() == 2) {
	        	cout << casamentos[i].first << " ----"<< casamentos[i].second << " ";
	        }else{
	        	if ( casamentos[i].second.size() == 2){
	        		cout << casamentos[i].first << "----"<< casamentos[i].second << "  ";
	        	}else{
	        		cout << casamentos[i].first << "----"<< casamentos[i].second << " ";
	        	}
	        }
	    }
        if (j == 6 ){
        	cout << endl;
        	j = 0;
        }
    }
}

int main(){
    vector<Professor> prof;
    vector<Escola> esc;
    criaListaPref(prof,esc);
    galeShapley(prof,esc);
    printCasamento();
    cout << endl << " " << casamentos.size() << " Emparelhamentos maximos" << endl;
}
