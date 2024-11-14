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

static void print_no(struct dht &dht, uint32_t id){
    auto it = dht.nos.find(id);
    std::cout << it->first << " {";
    for(uint32_t i = 0; i < log2(dht.id_max); ++i){
        if(it->second.finger_table[i] != 0){
            if(it->second.finger_table[i+1] == 0){
                std::cout << it->second.finger_table[i];
                break;
            } else {
                std::cout << it->second.finger_table[i] << ",";
            }
        }
    }
    std:: cout << "}\n";
}

void update_finger_table(struct dht &dht){
    for(auto it = dht.nos.begin(); it != dht.nos.end(); it++){
        uint32_t id = it->first;
        uint32_t m = log2(dht.id_max)+1;
        for(uint32_t i = 0; i < m; ++i){
            // procura pelo primeiro nó que é maior ou igual
            auto lb = dht.nos.lower_bound(((id + (1<<(i))) % (1<<(m))));
            // pega a partir do começo do anel
            if(lb == dht.nos.end()) lb = dht.nos.begin();
            dht.nos[id].finger_table[i] = lb->first;
        }
    }
}

void insere_no(uint32_t id, struct dht &dht){
    struct no novo_no;
    novo_no.id = id;
    dht.nos[id] = novo_no;

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

void remove_no(uint32_t id, struct dht &dht){
    auto excluido = dht.nos.find(id);
    auto it = dht.nos.end();
    // se é o último nó do anel, checamos se tem um diferente no início
    if(next(excluido, 1) == dht.nos.end() && excluido != dht.nos.begin()){
        it = dht.nos.begin();
    // se não tem próximo 
    } else {
        it =  next(excluido, 1);
    }

    if(it != dht.nos.end()){
        it->second.chaves.insert(excluido->second.chaves.begin(), excluido->second.chaves.end());
    }

    dht.nos.erase(id);

    if(dht.id_max == id) dht.id_max = dht.nos.rbegin()->first; 
    
    update_finger_table(dht);
}

void imprime_log(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht, std::vector<uint32_t> &lookup_ids){
    std::cout << timestamp << " L " << chave << " {";
    for(uint32_t i = 0; i < lookup_ids.size(); ++i){
        if(i == lookup_ids.size()-1)std:: cout << lookup_ids[i];
        else std:: cout << lookup_ids[i] << ",";
    }  
    std::cout << "}\n";

    for(uint32_t i = 0; i < lookup_ids.size(); ++i){
        std::cout << timestamp << " T ";
        print_no(dht, lookup_ids[i]);
    }
}

std::vector<uint32_t> lookup(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht){
    uint32_t passo = chave - id, cur_no = id;
    std::vector<uint32_t> lookup_ids;
    // caminha a passos da maior potência de 2 menor ou igual que falta para chegar no nó
    while(!(dht.nos.lower_bound(chave)->first == cur_no)){
        if(cur_no == 0) cur_no = dht.nos.begin()->first;
        if(cur_no == dht.nos.begin()->first && chave > dht.id_max){
            break;
        }
        lookup_ids.push_back(cur_no);
        cur_no = dht.nos[cur_no].finger_table[(uint32_t)log2(chave - cur_no)];
    }
    lookup_ids.push_back(cur_no);

    return lookup_ids;
}

void lookup_e_imprime(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht){
    std::vector<uint32_t> nos;
    nos = lookup(id, chave, timestamp, dht);
    imprime_log(id, chave, timestamp, dht, nos);
}

void inclui_chave(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht){
    std::vector<uint32_t> nos;
    uint32_t no_id;
    nos = lookup(id, chave, timestamp, dht);
    no_id = nos[nos.size() - 1];
    dht.nos[no_id].chaves.insert(chave);
}

