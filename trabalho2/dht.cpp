/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "dht.h"

void inicializa_dht(struct dht &dht){
    dht.id_max = 0;
}

static void print_no(struct dht &dht, int id){
    auto it = dht.nos.find(id);
    std::cout << it->first << " {";
    for(int i = 0; i < log2(dht.id_max)+1; ++i){
        if(i+1 == (int)log2(dht.id_max)+1){
            std::cout << it->second.finger_table[i];
            break;
        } else {
            std::cout << it->second.finger_table[i] << ",";
        }
    }
    std:: cout << "}\n";
}

void update_finger_table(struct dht &dht){
    for(auto it = dht.nos.begin(); it != dht.nos.end(); it++){
        int id = it->first;
        int m = log2(dht.id_max)+1;
        for(int i = 0; i < m; ++i){
            // procura pelo primeiro nó que é maior ou igual
            // utilizando a fórmula para calcular a finger table 
            auto lb = dht.nos.lower_bound(((id + (1<<(i))) % (1<<(m))));
            // pega a partir do começo do anel
            if(lb == dht.nos.end()) lb = dht.nos.begin();
            dht.nos[id].finger_table[i] = lb->first;
        }
    }
}

void insere_no(int id, struct dht &dht){
    struct no novo_no;
    novo_no.id = id;
    dht.nos[id] = novo_no;

    // com mais de 2 nós, estamos inserindo entre 2 deles
    if(dht.nos.size() > 1){
        auto prox = dht.nos.lower_bound(id+1);
        // pega a partir do começo do anel
        if(prox == dht.nos.end()) prox = dht.nos.begin();

        auto anterior = std::prev(prox);
        // se é o primeiro, o anterior vem no fim do anel
        if(anterior == dht.nos.end()) anterior = std::prev(anterior);

        for(auto chave : prox->second.chaves){
            if(chave > anterior->second.id && chave <= id){
                novo_no.chaves.insert(chave);
                prox->second.chaves.erase(chave);
            }
        }
    // com um único nó dividimos as chaves de acordo com a direção do primeiro nó
    } else if(dht.nos.size() == 1){
        auto unico = dht.nos.begin();
        if(id > unico->second.id){
            for(auto chave : unico->second.chaves){
                if(chave > unico->second.id && chave <= id){
                    novo_no.chaves.insert(chave);
                    unico->second.chaves.erase(chave);
                }
            }
        } else if (id < unico->second.id) {
            for(auto chave : unico->second.chaves){
                if(chave > unico->second.id || chave <= id){
                    novo_no.chaves.insert(chave);
                    unico->second.chaves.erase(chave);
                }
            }
        }
    }

    dht.id_max = std::max(id, dht.id_max);

    update_finger_table(dht);
}

void remove_no(int id, struct dht &dht){
    auto excluido = dht.nos.find(id);
    auto it = dht.nos.end();
    // se é o último nó do anel, checamos se tem um diferente no início
    if(next(excluido, 1) == dht.nos.end() && excluido != dht.nos.begin()){
        it = dht.nos.begin();
    // se não tem próximo 
    } else {
        it =  next(excluido, 1);
    }
    // transferimos as chaves
    if(it != dht.nos.end()){
        it->second.chaves.insert(excluido->second.chaves.begin(), excluido->second.chaves.end());
    }

    dht.nos.erase(id);

    // atualizamos o maior valor
    if(dht.id_max == id) dht.id_max = dht.nos.rbegin()->first; 
    
    update_finger_table(dht);
}

void imprime_log(int id, int chave, int timestamp, struct dht &dht, std::vector<int> &lookup_ids){
    std::cout << timestamp << " L " << chave << " {";
    for(unsigned i = 0; i < lookup_ids.size(); ++i){
        if(i == lookup_ids.size()-1)std:: cout << lookup_ids[i];
        else std:: cout << lookup_ids[i] << ",";
    }  
    std::cout << "}\n";

    for(unsigned i = 0; i < lookup_ids.size(); ++i){
        std::cout << timestamp << " T ";
        print_no(dht, lookup_ids[i]);
    }
}

std::vector<int> lookup(int id, int chave, int timestamp, struct dht &dht){
    int cur_no = id, m = log2(dht.id_max);
    if(log2(dht.id_max) != (int)log2(dht.id_max)) m++;

    // se a chave estiver antes do nó corrente, precisamos tratá-la como
    // um valor maior, um valor com um passo de 2^m
    int chave_virtual = chave - cur_no;
    if(chave_virtual < 0){
        chave_virtual = (chave_virtual + (1<<m));
    }

    std::vector<int> lookup_ids;
    // caminha a passos da maior potência de 2 menor ou igual que falta para chegar no nó
    while(!(dht.nos.lower_bound(chave)->first == cur_no)){
        if(cur_no == 0) cur_no = dht.nos.begin()->first;
        // demos a volta na DHT e chegamos às chaves que o primeiro nó
        // carrega, além daquelas do intervalo (1..n_id)
        if(cur_no == dht.nos.begin()->first && chave > dht.id_max){
            break;
        }
        lookup_ids.push_back(cur_no);
        cur_no = dht.nos[cur_no].finger_table[(int)log2((chave_virtual))];
        // atualizamos a chave virtual na mesma lógica anterior
        chave_virtual = chave - cur_no;
        if(chave_virtual < 0){
            chave_virtual = (chave_virtual + (1<<m));
        }
    }
    lookup_ids.push_back(cur_no);

    return lookup_ids;
}

void lookup_e_imprime(int id, int chave, int timestamp, struct dht &dht){
    std::vector<int> nos;
    nos = lookup(id, chave, timestamp, dht);
    imprime_log(id, chave, timestamp, dht, nos);
}

void inclui_chave(int id, int chave, int timestamp, struct dht &dht){
    std::vector<int> nos;
    int no_id;
    nos = lookup(id, chave, timestamp, dht);
    // inserimos a chave no último nó visitado pelo lookup
    no_id = nos[nos.size() - 1];
    dht.nos[no_id].chaves.insert(chave);
}

