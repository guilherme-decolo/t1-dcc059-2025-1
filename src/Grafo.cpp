#include "Grafo.h"


Grafo::Grafo(bool direcionado, bool ponderado_aresta, bool ponderado_vertice) {
    grafo_ordem = 0;
    grafo_direcionado = direcionado;
    grafo_ponderado_aresta = ponderado_aresta;
    grafo_ponderado_vertice = ponderado_vertice;          
}

Grafo::~Grafo() {
}

Grafo::ler_arquivo_entrada(const string& nome_arquivo) {
  
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return false;
    }

    // Lendo primeira linha
    int dir, pond_aresta, pond_vertice;
    arquivo >> dir >> pond_aresta >> pond_vertice;

    direcionado = dir;
    ponderado_aresta = pond_aresta;
    ponderado_vertice = pond_vertice;

    // Lendo segunda linha
    int ordem;
    arquivo >> ordem;
    lista_adj.clear(); // Zerando grafo
    unordered_map<char,No*> mapa_nos; // Cria um mapa de nós que associa um ID para cada nó
                                        // Facilita pra quando for ler as arestas

    // Lendo nós
    for(int i = 0; i < ordem; i++){
        char id;
        arquivo >> id;
        No* no = new No(id); //CRIAR CONSTRUTOR 
        lista_adj.push_back(no);
        mapa_nos[id] = no;
    }

    // Lendo arestas
    char origem, destino;
    int peso;
    
    while(arquivo >> origem >> destino){

        if(pond_aresta){
            arquivo >> peso;
        }
        else peso = 1;
        
        No* no_origem = mapa_nos[origem];
        No* no_destino = mapa_nos[destino];

        no_origem->AdicionarVizinho(no_destino, peso); //CRIAR MÉTODO

        if(!direcionado){
            no_destino->AdicionarVizinho(no_origem, peso);
        }
    }
    return false;
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
