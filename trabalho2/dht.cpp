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

int inicializa_dht(struct dht &dht){
    dht.no_max = 0;
    return 1;
}

void print_nos(struct dht &dht){
    for(auto it = dht.nos.begin(); it != dht.nos.end(); it++){
        // std::cout << it->first << " finger table \n";
        std::cout << it->first << " {";
        for(int i = 0; i < log2(dht.no_max); ++i){
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
}

void print_no(struct dht &dht, int id){
    auto it = dht.nos.find(id);
    std::cout << it->first << " {";
    for(int i = 0; i < log2(dht.no_max); ++i){
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
        uint32_t m = log2(dht.no_max)+1;
        for(uint32_t i = 0; i < m; ++i){
            auto lb = dht.nos.lower_bound(((id + (1<<(i))) % (1<<(m))));
            if(lb == dht.nos.end()) lb = dht.nos.begin();
            dht.nos[id].finger_table[i] = lb->first;
        }
    }
}

void insere_no(uint32_t id, struct dht &dht){
    dht.no_max = std::max(id, dht.no_max);

    struct no novo_no;
    novo_no.id = id;
    dht.nos[id] = novo_no;

    update_finger_table(dht);
}

void remove_no(int id, struct dht &dht){
    if(dht.no_max == id) dht.no_max = dht.nos.rbegin()->first; 

    dht.nos.erase(id);

    update_finger_table(dht);
    // falta transferir chaves
}

void lookup(int id, int chave, struct dht &dht, int timestamp){
    uint32_t passo = chave - id, cur_no = id;
    std::vector<int> lookup_ids;
    // if(passo < 0) passo += dht.no_max; passo negativo?
    while(cur_no < chave){
        lookup_ids.push_back(cur_no);
        cur_no = dht.nos[cur_no].finger_table[(int)log2(passo)];
        passo -= 1 << (int)log2(passo);
    }
    lookup_ids.push_back(cur_no);

    std::cout << timestamp << " L " << chave << " {";
    for(int i = 0; i < lookup_ids.size(); ++i){
        if(i == lookup_ids.size()-1)std:: cout << lookup_ids[i];
        else std:: cout << lookup_ids[i] << ",";
    }  
    std::cout << "}\n";

    for(int i = 0; i < lookup_ids.size(); ++i){
        print_no(dht, lookup_ids[i]);
    }  
    
}


void inclui_chave(int id, int chave, struct dht &dht){
}

