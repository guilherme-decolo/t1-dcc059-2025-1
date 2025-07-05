#include "No.h"
#include "Aresta.h"

//CRIAR MÉTODO ADICIONARVIZINHO
        
No::No(char no_id){
    id = no_id;
    peso = 0;
}

No::~No(){
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
    arestas.clear();
}

void No::AdicionarVizinho(char id_vizinho, int peso_aresta){
    // Cria uma nova aresta passando o ID do vizinho e o peso
    Aresta* nova_aresta = new Aresta(id_vizinho, peso_aresta);
    
    arestas.push_back(nova_aresta);
}