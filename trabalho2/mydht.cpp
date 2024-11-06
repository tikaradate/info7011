/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <iostream>
#include "dht.h"


int main(){
    std::string timestamp, operacao, id, chave;
    struct dht dht;
    inicializa_dht(dht);
    while(std::cin >> timestamp >> operacao >> id >> chave){
        if(operacao == "E"){
            insere_no(stoi(id), dht);
        } else if (operacao == "S"){
            remove_no(stoi(id), dht);
        } else if (operacao == "I"){
            inclui_chave(stoi(id), stoi(chave),stoi(timestamp), dht);
        } else if (operacao == "L"){
            lookup_e_imprime(stoi(id), stoi(chave), stoi(timestamp), dht);
        }
    }
}