/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>
#include <map>
#include <set>

#define MAX 20 //2^20 ~= 10^6

struct no {
    int id;
    int finger_table[MAX] = {-1};
    std::set<int> chaves;
};

struct dht{
    std::map<int, struct no> nos;
    int id_max;
};

void inicializa_dht(struct dht &dht);

void insere_no(int id, struct dht &dht);

void remove_no(int id, struct dht &dht);

void inclui_chave(int id, int chave, int timestamp, struct dht &dht);

void lookup_e_imprime(int id, int chave, int timestamp, struct dht &dht);
