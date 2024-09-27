/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <iostream>
#include "hashtable.h"

#define M 11

int main(){
    std::string operacao;
    int valor;
    cuckoo_hash cuckoo;

    inicializa_cuckoo(cuckoo, M);
    while(std::cin >> operacao >> valor){
        if(operacao == "i"){
            inserir(cuckoo, valor);
        } else if (operacao == "r"){
            remover(cuckoo, valor);
        }
    }

    imprimir(cuckoo);
}