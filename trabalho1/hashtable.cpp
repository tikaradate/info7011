/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "hashtable.h"

// Funções hash de cada tabela

int hash_1(int k, int m){
    return k%m;
}

int hash_2(int k, int m){
    return (int) (m*(k*0.9 - (int)(k*0.9)));
}


void inicializa_cuckoo(cuckoo_hash &cuckoo, int tamanho){
    cuckoo.m = tamanho;
    cuckoo.t1.tabela.resize(cuckoo.m, elemento());
    cuckoo.t2.tabela.resize(cuckoo.m, elemento());
}

int busca(cuckoo_hash cuckoo, int valor){
    unsigned pos1 = hash_1(valor, cuckoo.m); 
    unsigned pos2 = hash_2(valor, cuckoo.m);

    if(cuckoo.t1.tabela[pos1].valor == valor) return pos1;

    if(cuckoo.t2.tabela[pos2].valor == valor) return pos2;

    return -1;

}

void inserir(cuckoo_hash &cuckoo, int valor){
    int pos1 = hash_1(valor, cuckoo.m);

    // checamos se existe espaco para o elemento i_k na primeira tabela
    if(cuckoo.t1.tabela[pos1].vazio || cuckoo.t1.tabela[pos1].excluido){
        cuckoo.t1.tabela[pos1].valor = valor;
        cuckoo.t1.tabela[pos1].vazio = false;
        cuckoo.t1.tabela[pos1].excluido = false;
    // se não existir, movemos o elemento i_j da primeira tabela para a segunda
    // e inserimos o elemento i_k na primeira tabela 
    } else {
        elemento e = cuckoo.t1.tabela[pos1];

        int pos2 = hash_2(e.valor, cuckoo.m);

        cuckoo.t2.tabela[pos2] = e;

        cuckoo.t1.tabela[pos1].valor = valor;
        cuckoo.t1.tabela[pos1].vazio = false;
        cuckoo.t1.tabela[pos1].excluido = false;
    }
}

void remover(cuckoo_hash &cuckoo, int valor){
    int pos1 = hash_1(valor, cuckoo.m);
    int pos2 = hash_2(valor, cuckoo.m);

    if(!cuckoo.t2.tabela[pos2].vazio && cuckoo.t2.tabela[pos2].valor == valor){
        cuckoo.t2.tabela[pos2].excluido = true; 
    } else if(!cuckoo.t1.tabela[pos1].vazio && cuckoo.t1.tabela[pos1].valor == valor){
        cuckoo.t1.tabela[pos1].excluido = true; 
    }
}

void imprimir(struct cuckoo_hash cuckoo){
    std::vector<std::tuple<int, int, int>> impressao;

    for(int i = 0; i < cuckoo.m; ++i){
        if(!cuckoo.t1.tabela[i].vazio && !cuckoo.t1.tabela[i].excluido){
            std::tuple<int, int, int> cur (cuckoo.t1.tabela[i].valor, 1, hash_1(cuckoo.t1.tabela[i].valor, cuckoo.m));
            impressao.push_back(cur);
        }
        if(!cuckoo.t2.tabela[i].vazio && !cuckoo.t2.tabela[i].excluido){
            std::tuple<int, int, int> cur (cuckoo.t2.tabela[i].valor, 2, hash_2(cuckoo.t2.tabela[i].valor, cuckoo.m));
            impressao.push_back(cur);
        }
    }
    
    // utilizando o fato de que as tuplas do C++ fazem comparação de tupla campo a campo
    sort(impressao.begin(), impressao.end());

    for(unsigned int i = 0; i < impressao.size(); ++i){
        std::cout << std::get<0>(impressao[i]) << ',' << 'T' << std::get<1>(impressao[i]) << ',' << std::get<2>(impressao[i]) << std::endl;
    }
    
}