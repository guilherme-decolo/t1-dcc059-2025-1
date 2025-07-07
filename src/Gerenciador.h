#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);

    
    static void imprimir_vetor_char(const vector<char>& vetor, ostream& saida);
    static void imprimir_grafo_adj(Grafo* grafo, ostream& saida);
};


#endif //GERENCIADOR_H
