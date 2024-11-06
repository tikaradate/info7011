/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>
#include <map>

// struct finger_table{
//     std::vector<int> prox_no(12, -1);
// };

struct no {
    uint32_t id;
    // struct finger_table table;
    uint32_t finger_table[12] = {0};
    std::vector<uint32_t> chaves;
};

struct dht{
    std::map<uint32_t, struct no> nos;
    uint32_t no_max;
};


int inicializa_dht(struct dht &dht);

void print_nos(struct dht &dht);

void insere_no(uint32_t id, struct dht &dht);

void remove_no(int id, struct dht &dht);

void inclui_chave(int id, int chave, struct dht &dht);

void lookup(int id, int chave, struct dht &dht, int timestamp);
