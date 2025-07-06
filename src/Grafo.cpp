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
    // Verifica se os nós de origem e destino existem no grafo -------------------------------------------
    No* no_origem = nullptr;
    No* no_destino = nullptr;
    for (No* no : this->lista_adj) {
        if (no->id == id_no_a) { no_origem = no;}
        else if (no->id == id_no_b) { no_destino = no;}
        if (no_origem != nullptr && no_destino != nullptr) { break;}
    }
    if (no_origem == nullptr) {
        cout << "Erro: O no de origem '" << id_no_a << "' nao existe no grafo." << endl;
        return {};
    }
    if (no_destino == nullptr) {
        cout << "Erro: O no de destino '" << id_no_b << "' nao existe no grafo." << endl;
        return {};
    }
    // ---------------------------------------------------------------------------------------------------

    map<char, int> distancias;      // Armazena as distâncias de cada nó a partir da origem
    map<char, char> predecessores;  // Armazena os predecessores de cada nó no caminho mais curto
    map<char, bool> visitados;      // Controle dos nós já visitados {S}

    // Inicializa as distancias como infinito(valor muito alto) para todos os nós.
    int inf = 100000;
    for (No* no : this->lista_adj) { 
        distancias[no->id] = inf;
        predecessores[no->id] = 0; 
        visitados[no->id] = false;
    }

    distancias[id_no_a] = 0;            // Define a distância do nó de origem para ele mesmo como 0.

    for (int count = 0; count < this->ordem; ++count) {

        int menor_distancia = inf;
        char id_no_j = 0; // Aux para guardar o ID do nó com a menor distância

        for (auto const& par : distancias) {
            char id = par.first;
            int dist = par.second;

            if (!visitados[id] && dist < menor_distancia) { // Adicionei a verificação !visitados[id] que estava faltando no seu loop de busca manual
                menor_distancia = dist;
                id_no_j = id;
            }
        }
        // Se não encontramos nenhum nó alcançável (id_no_j continua 0), podemos parar.
        if (id_no_j == 0) {break;}

        // Se o nó com menor distância é o nosso destino, podemos parar antes.
        if (id_no_j == id_no_b) {break;}

        visitados[id_no_j] = true;
        No* no_j = nullptr;
        for (No* no : this->lista_adj) {
            if (no->id == id_no_j) {
                no_j = no;
                break;
            }
        }
        if (no_j == nullptr) { continue; }

        for (Aresta* aresta : no_j->arestas) {
            char id_no_i = aresta->id_no_alvo;
            int custo_ji = aresta->peso;
            int distancias_estrela = distancias[id_no_j] + custo_ji;

            if (distancias_estrela < distancias[id_no_i]) {
                distancias[id_no_i] = distancias_estrela;
                predecessores[id_no_i] = id_no_j;
            }
        }
    }

    // Caminho de volta
    vector<char> caminho;
    char id_atual = id_no_b;

    if (distancias[id_no_b] == inf) {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    while (id_atual != 0) {
        caminho.push_back(id_atual);
        if (id_atual == id_no_a) break;
        id_atual = predecessores[id_atual];
    }
    
    reverse(caminho.begin(), caminho.end());

    if (caminho.empty() || caminho.front() != id_no_a) {
         cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }
    
    cout << "Caminho minimo encontrado com custo " << distancias[id_no_b] << endl;
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b) {
    // Verifica se os nós de origem e destino existem no grafo -------------------------------------------
    No* no_origem = nullptr;
    No* no_destino = nullptr;
    for (No* no : this->lista_adj) {
        if (no->id == id_no_a) { no_origem = no;}
        else if (no->id == id_no_b) { no_destino = no;}
        if (no_origem != nullptr && no_destino != nullptr) { break;}
    }
    if (no_origem == nullptr) {
        cout << "Erro: O no de origem '" << id_no_a << "' nao existe no grafo." << endl;
        return {};
    }
    if (no_destino == nullptr) {
        cout << "Erro: O no de destino '" << id_no_b << "' nao existe no grafo." << endl;
        return {};
    }
    // ---------------------------------------------------------------------------------------------------
    
    int inf = 100000; // Um valor grande para representar infinito

    map<char, map<char, int>> dist;     // Matriz Distâncias
    map<char, map<char, char>> pred;    // Matriz Predecessores

    // Inicializa a matriz distamância e predecessores.
    for (No* no_i : this->lista_adj) {
        for (No* no_j : this->lista_adj) {
            char i = no_i->id;
            char j = no_j->id;
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = inf;
            }
            pred[i][j] = -1;
        }
    }

    for (No* no_atual : this->lista_adj) {
        for (Aresta* aresta : no_atual->arestas) {
            char u = no_atual->id;
            char v = aresta->id_no_alvo;
            dist[u][v] = aresta->peso;
            pred[u][v] = u;
        }
    }

    for (No* no_k : this->lista_adj) {
        char k = no_k->id;
        for (No* no_i : this->lista_adj) {
            char i = no_i->id;
            for (No* no_j : this->lista_adj) {
                char j = no_j->id;
                if (dist[i][k] != inf && dist[k][j] != inf && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }

    // Caminho de volta
    vector<char> caminho;

    if (dist[id_no_a][id_no_b] == inf) {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    char atual = id_no_b;
    while (atual != -1 && pred[id_no_a][atual] != -1) {
        caminho.push_back(atual);
        atual = pred[id_no_a][atual];
    }
    
    if(atual != -1) caminho.push_back(atual);   // Adiciona o nó de origem, que é o último a ser encontrado

    reverse(caminho.begin(), caminho.end());    // Inverte o caminho, pois ele foi construído do destino para a origem
    
    // Verificação final para garantir que o caminho é válido
    if (caminho.empty() || caminho.front() != id_no_a) {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    cout << "Caminho minimo encontrado com custo " << dist[id_no_a][id_no_b] << endl;
    return caminho;
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

// Funções auxiliares para a Arvore Geradora Mínima Kruskal
char encontrar_raiz(char no, std::map<char, char>& pai) {
    if (pai[no] == no) {
        return no;
    }
    pai[no] = encontrar_raiz(pai[no], pai);
    return pai[no];
}

void unir_conjuntos(char u, char v, std::map<char, char>& pai) {
    char raiz_u = encontrar_raiz(u, pai);
    char raiz_v = encontrar_raiz(v, pai);
    if (raiz_u != raiz_v) {
        pai[raiz_u] = raiz_v;
    }
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    if (ids_nos.empty()) {
        cout << "Erro: O subconjunto de nos nao pode ser vazio." << endl;
        return nullptr;
    }

    std::map<int, std::vector<std::pair<char, char>>> arestas_por_peso;
    std::map<char, std::map<char, bool>> aresta_adicionada;

    for (char id_no : ids_nos) {
        No* no_atual = mapa_de_nos_por_id[id_no];
        for (Aresta* aresta : no_atual->arestas) {
            bool vizinho_no_subgrafo = false;
            for (char id_vizinho_verificar : ids_nos) {
                if (aresta->id_no_alvo == id_vizinho_verificar) {
                    vizinho_no_subgrafo = true;
                    break;
                }
            }

            if (vizinho_no_subgrafo) {
                char u = id_no;
                char v = aresta->id_no_alvo;
                
                if (u > v) {
                    char temp = u;
                    u = v;
                    v = temp;
                }

                if (!aresta_adicionada[u][v]) {
                    arestas_por_peso[aresta->peso].push_back({u, v});
                    aresta_adicionada[u][v] = true;
                }
            }
        }
    }

    std::map<char, char> pai;
    for (char id : ids_nos) {
        pai[id] = id;
    }

    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->ordem = ids_nos.size();
    for (char id_no : ids_nos) {
        No* no_novo = new No(id_no);
        agm->lista_adj.push_back(no_novo);
        agm->mapa_de_nos_por_id[id_no] = no_novo;
    }

    int num_arestas_agm = 0;
    bool arvore_completa = false; 

    for (auto const& par_peso : arestas_por_peso) {
        int peso = par_peso.first;
        for (auto const& par_nos : par_peso.second) {
            char u = par_nos.first;
            char v = par_nos.second;

            if (encontrar_raiz(u, pai) != encontrar_raiz(v, pai)) {
                unir_conjuntos(u, v, pai);
                agm->mapa_de_nos_por_id[u]->AdicionarVizinho(v, peso);
                agm->mapa_de_nos_por_id[v]->AdicionarVizinho(u, peso);
                num_arestas_agm++;

                if (num_arestas_agm == ids_nos.size() - 1) {
                    arvore_completa = true;
                    break;
                }
            }
        }
        if (arvore_completa) {
            break;
        }
    }

    if (num_arestas_agm < ids_nos.size() - 1) {
        cout << "Aviso: Nao foi possivel gerar uma arvore conectada com os nos fornecidos." << endl << endl;
    }
    else{
        agm->imprimirGrafo();
    }

    return agm;
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

void Grafo::imprimirGrafo() {
    cout << "\n--- DETALHES DO GRAFO ---" << endl;
    cout << "Ordem: " << ordem << endl;
    cout << "Direcionado: " << (in_direcionado ? "Sim" : "Nao") << endl;  // Operador ternário
    cout << "Ponderado nas Arestas: " << (in_ponderado_aresta ? "Sim" : "Nao") << endl;
    cout << "Ponderado nos Vertices: " << (in_ponderado_vertice ? "Sim" : "Nao") << endl;

    cout << "\nLista de adjacência:" << endl;
    if (lista_adj.empty()) {
        cout << "Lista Vazia" << endl;
        return;
    }
    cout << "Representaçao: Vertice(Peso do vertice) -> Vizinho(peso da aresta),..." << endl;
    for (No* no : lista_adj) {
        cout << no->id;

        if (in_ponderado_vertice) {
            cout << "(" << no->peso << ")";
        }
        cout << " -> ";
        bool virgula = true;
        for (Aresta* aresta : no->arestas) {
            if (!virgula) {
                cout << ", ";
            }
            cout << aresta->id_no_alvo;
            if (in_ponderado_aresta) {
                cout << "(" << aresta->peso << ")";
            }
            virgula = false;
        }
        cout << endl;
    }
    cout << "-----------------------------------" << endl;
}
