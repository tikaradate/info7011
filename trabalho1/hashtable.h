/*
    Autor:     Vinicius Tikara Venturi Date
    Matrícula: 201900121201
*/

#include <vector>

struct elemento {
    int valor = -1;
    bool vazio = true;
    bool excluido = false;
};

struct tabela_hash{
    std::vector<elemento> tabela;
};

struct cuckoo_hash{
    int m;
    tabela_hash t1;
    tabela_hash t2;
};

int hash_1(int k, int m);

int hash_2(int k, int m);

void inicializa_cuckoo(cuckoo_hash &uckoo, int tamanho);

int busca(cuckoo_hash cuckoo, int valor);

void inserir(cuckoo_hash &cuckoo, int valor);

void remover(cuckoo_hash &cuckoo, int valor);

void imprimir(cuckoo_hash cuckoo);