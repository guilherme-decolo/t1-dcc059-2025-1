#include "Grafo.h"
#include <fstream>
#include <unordered_map> 
#include <map> 
#include <algorithm> 



Grafo::Grafo() {
    ordem = 0;
    in_direcionado = false;
    in_ponderado_aresta = false;
    in_ponderado_vertice = false;          
}

Grafo::~Grafo() {
    for (No* no : lista_adj) {
        delete no;
    }
    lista_adj.clear();
    mapa_de_nos_por_id.clear();
}

bool Grafo::ler_arquivo_entrada(const string& nome_arquivo) {
  
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return false;
    }

    // Limpa o grafo existente
    for (No* no : lista_adj) {
        delete no; 
    }
    lista_adj.clear();
    mapa_de_nos_por_id.clear();
    this->ordem = 0; 

    // Lendo primeira linha
    int direc, pond_aresta, pond_vertice;
    if (!(arquivo >> direc >> pond_aresta >> pond_vertice)) { 
        cout << "Erro: Nao foi possivel ler o tipo de grafo na primeira linha" << endl;
        arquivo.close();
        return false;
    }

    this->in_direcionado = (direc == 1);
    this->in_ponderado_aresta = (pond_aresta == 1);
    this->in_ponderado_vertice = (pond_vertice == 1);

    // Lendo segunda linha
    if (!(arquivo >> this->ordem)) {
        cout << "Erro: Nao foi possivel ler a ordem do grafo" << endl;
        arquivo.close();
        return false;
    }

    // Lendo nós
    for(int i = 0; i < this->ordem; i++){
        char id;
        if (!(arquivo >> id)) { 
            cout << "Erro: Nao foi possivel ler o ID do vertice " << i+1  << endl;
            arquivo.close();
            return false;
        }

        No* no = new No(id);

        if (this->in_ponderado_vertice) {
            int peso_vertice;
            if (!(arquivo >> peso_vertice)) { 
                cout << "Erro: Nao foi possivel ler o peso do vertice " << id << endl;
                arquivo.close();
                delete no; 
                return false;
            }
            no->peso = peso_vertice;
        } else {
            no->peso = 0;
        }

        lista_adj.push_back(no);
        mapa_de_nos_por_id[id] = no;
    }

    // Lendo arestas
    char origem, destino;
    int peso;
    
    while(arquivo >> origem >> destino){

        if(in_ponderado_aresta){
            if (!(arquivo >> peso)) { 
                cout << "Erro: Nao foi possivel ler o peso da aresta entre " 
                     << origem << " e " << destino << endl;
                arquivo.close();
                return false;
            }
        }
        else peso = 1;
        
        No* no_origem = nullptr;
        No* no_destino = nullptr;

        // Verificando se os nós existem no mapa
        if (mapa_de_nos_por_id.count(origem)) {
            no_origem = mapa_de_nos_por_id[origem]; // Aponta pro nó
        }
        if (mapa_de_nos_por_id.count(destino)) {
            no_destino = mapa_de_nos_por_id[destino];
        }

        // Verificando se existem
        if (no_origem == nullptr || no_destino == nullptr) {
            cout << "Erro: Vertice(s) da aresta (" << origem << "," << destino 
                << ") nao encontrado(s) no grafo. Verifique o arquivo de entrada" << endl;
            arquivo.close();
            return false;
        }

        // Criando conexão
        no_origem->AdicionarVizinho(destino, peso); 

        if(!this->in_direcionado){ 
            no_destino->AdicionarVizinho(origem, peso); 
        }
    }
    arquivo.close(); 
    return true;
}

void Grafo::busca_profundidade(No* no_atual, map<char, bool>& visitado, vector<char>& nos_alcancaveis) {
    visitado[no_atual->id] = true;
    nos_alcancaveis.push_back(no_atual->id);

    for (Aresta* aresta : no_atual->arestas) {
        char vizinho_id = aresta->id_no_alvo;
        
        // Verifica se o vizinho existe no grafo e se não foi visitado
        if (mapa_de_nos_por_id.count(vizinho_id) && !visitado[vizinho_id]) { 
            No* no_vizinho = mapa_de_nos_por_id[vizinho_id];
            busca_profundidade(no_vizinho, visitado, nos_alcancaveis); 
        }
    }
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
