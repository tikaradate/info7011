/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>
#include <map>
#include <set>

#define MAX 20 //2^20 ~= 10^6

struct no {
    uint32_t id;
    uint32_t finger_table[MAX] = {0};
    std::set<uint32_t> chaves;
};

struct dht{
    std::map<uint32_t, struct no> nos;
    uint32_t id_max;
};

void inicializa_dht(struct dht &dht);

void insere_no(uint32_t id, struct dht &dht);

void remove_no(uint32_t id, struct dht &dht);

void inclui_chave(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht);

void lookup_e_imprime(uint32_t id, uint32_t chave, uint32_t timestamp, struct dht &dht);
