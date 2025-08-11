#ifndef GULOSOS_H
#define GULOSOS_H

#include "Grafo.h"
#include <vector>
#include <set>

using namespace std;

class Gulosos {
public:
    // O construtor recebe um ponteiro para o grafo principal
    Gulosos(Grafo* grafo);

    // Métodos públicos para cada um dos algoritmos solicitados
    vector<char> guloso();
    vector<char> g_rand_adapt(int max_iteracoes, double alfa);
    vector<char> g_rand_adapt_reativo(int max_iteracoes, const vector<double>& alfas);

private:
    Grafo* grafo; // Ponteiro para o grafo do usuário

    // Constantes para representar o status de um nó, para legibilidade
    static const int BRANCO = 0;
    static const int CINZA = 1;
    static const int PRETO = 2;

    // --- Funções Auxiliares ---

    // Função principal que constrói um conjunto dominante (ainda não conectado)
    set<char> construir_conjunto_dominante(double alfa);

    // Função que conecta os componentes do conjunto dominante
    void conectar_componentes(set<char>& conjunto_dominante);

    // Função que verifica se o subgrafo induzido por um conjunto de nós é conectado
    bool eh_conectado(const set<char>& nos);
    
    // DFS adaptada para verificar a conectividade em um subconjunto de nós
    void busca_profundidade(char id_no, const set<char>& nos, set<char>& visitados);
};

#endif // GULOSOS_H
