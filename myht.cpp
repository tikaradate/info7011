#include <iostream>

#include "hashtable.h"

#define M 11

int main(){
    std::string operacao;
    int valor;
    cuckoo_hash cuckoo;
    inicializa_cuckoo(cuckoo, M);
    while(std::cin >> operacao >> valor){
        // std::cout << "Tabela atual------" << std::endl;
        // imprimir(cuckoo);
        // std::cout << "Tabela atual------" << std::endl;

        if(operacao == "i"){
            inserir(cuckoo, valor);
        } else if (operacao == "r"){
            remover(cuckoo, valor);
        }

        // std::cout << std::endl;

    }

    imprimir(cuckoo);
}