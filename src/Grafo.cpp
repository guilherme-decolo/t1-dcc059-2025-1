#include "Grafo.h"
#include <fstream>
#include <unordered_map>
#include <map>
#include <algorithm>

Grafo::Grafo()
{
    ordem = 0;
    in_direcionado = false;
    in_ponderado_aresta = false;
    in_ponderado_vertice = false;
}

Grafo::~Grafo()
{
    for (No *no : lista_adj)
    {
        delete no;
    }
    lista_adj.clear();
    mapa_de_nos_por_id.clear();
}

bool Grafo::ler_arquivo_entrada(const string &nome_arquivo)
{

    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return false;
    }

    // Limpa o grafo existente
    for (No *no : lista_adj)
    {
        delete no;
    }
    lista_adj.clear();
    mapa_de_nos_por_id.clear();
    this->ordem = 0;

    // Lendo primeira linha
    int direc, pond_aresta, pond_vertice;
    if (!(arquivo >> direc >> pond_aresta >> pond_vertice))
    {
        cout << "Erro: Nao foi possivel ler o tipo de grafo na primeira linha" << endl;
        arquivo.close();
        return false;
    }

    this->in_direcionado = (direc == 1);
    this->in_ponderado_aresta = (pond_aresta == 1);
    this->in_ponderado_vertice = (pond_vertice == 1);

    // Lendo segunda linha
    if (!(arquivo >> this->ordem))
    {
        cout << "Erro: Nao foi possivel ler a ordem do grafo" << endl;
        arquivo.close();
        return false;
    }

    // Lendo nós
    for (int i = 0; i < this->ordem; i++)
    {
        char id;
        if (!(arquivo >> id))
        {
            cout << "Erro: Nao foi possivel ler o ID do vertice " << i + 1 << endl;
            arquivo.close();
            return false;
        }

        No *no = new No(id);

        if (this->in_ponderado_vertice)
        {
            int peso_vertice;
            if (!(arquivo >> peso_vertice))
            {
                cout << "Erro: Nao foi possivel ler o peso do vertice " << id << endl;
                arquivo.close();
                delete no;
                return false;
            }
            no->peso = peso_vertice;
        }
        else
        {
            no->peso = 0;
        }

        lista_adj.push_back(no);
        mapa_de_nos_por_id[id] = no;
    }

    // Lendo arestas
    char origem, destino;
    int peso;

    while (arquivo >> origem >> destino)
    {

        if (in_ponderado_aresta)
        {
            if (!(arquivo >> peso))
            {
                cout << "Erro: Nao foi possivel ler o peso da aresta entre "
                     << origem << " e " << destino << endl;
                arquivo.close();
                return false;
            }
        }
        else
            peso = 1;

        No *no_origem = nullptr;
        No *no_destino = nullptr;

        // Verificando se os nós existem no mapa
        if (mapa_de_nos_por_id.count(origem))
        {
            no_origem = mapa_de_nos_por_id[origem]; // Aponta pro nó
        }
        if (mapa_de_nos_por_id.count(destino))
        {
            no_destino = mapa_de_nos_por_id[destino];
        }

        // Verificando se existem
        if (no_origem == nullptr || no_destino == nullptr)
        {
            cout << "Erro: Vertice(s) da aresta (" << origem << "," << destino
                 << ") nao encontrado(s) no grafo. Verifique o arquivo de entrada" << endl;
            arquivo.close();
            return false;
        }

        // Criando conexão
        no_origem->AdicionarVizinho(destino, peso);

        if (!this->in_direcionado)
        {
            no_destino->AdicionarVizinho(origem, peso);
        }
    }
    arquivo.close();
    return true;
}

void Grafo::busca_profundidade(No *no_atual, map<char, bool> &visitado, vector<char> &nos_alcancaveis)
{
    visitado[no_atual->id] = true;
    nos_alcancaveis.push_back(no_atual->id);

    for (Aresta *aresta : no_atual->arestas)
    {
        char vizinho_id = aresta->id_no_alvo;

        // Verifica se o vizinho existe no grafo e se não foi visitado
        if (mapa_de_nos_por_id.count(vizinho_id) && !visitado[vizinho_id])
        {
            No *no_vizinho = mapa_de_nos_por_id[vizinho_id];
            busca_profundidade(no_vizinho, visitado, nos_alcancaveis);
        }
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    // Verificando se o grafo é direcionado
    if (!in_direcionado)
    {
        cout << "Erro: Fecho transitivo direto so funciona paraa grafos direcionados." << endl;
        return {};
    }

    // Encontrando o nó inicial pelo seu ID usando o mapa de nós
    No *no_origem = nullptr;

    // Verificando se o nó existe no mapa
    if (mapa_de_nos_por_id.count(id_no))
    {
        no_origem = mapa_de_nos_por_id[id_no]; // Aponta pro nó
    }

    // Verificando se encontrou
    if (no_origem == nullptr)
    {
        cout << "Erro: No inicial '" << id_no << "' nao encontrado no grafo" << endl;
        return {};
    }

    // Mapa para controlar os nós visitados
    map<char, bool> visitado;

    // Inicializando todos os nos do grafo como nao visitados
    for (No *node : lista_adj)
    {
        visitado[node->id] = false;
    }

    // Armazenando os IDs dos nos alcancaveis
    vector<char> nos_alcancaveis;

    // Usando a função auxiliar pra BUSCA EM PROFUNDIDADE
    busca_profundidade(no_origem, visitado, nos_alcancaveis);

    return nos_alcancaveis;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    // Verificando se o grafo é direcionado
    if (!in_direcionado)
    {
        cout << "Erro: Fecho transitivo indireto so funciona paraa grafos direcionados." << endl;
        return {};
    }

    // Encontrando o nó alvo pelo seu ID usando o mapa de nós
    No *no_alvo = nullptr;

    // Verificando se o nó existe no mapa
    if (mapa_de_nos_por_id.count(id_no))
    {
        no_alvo = mapa_de_nos_por_id[id_no]; // Aponta para o nó
    }

    // Verificando se encontrou
    if (no_alvo == nullptr)
    {
        cout << "Erro: No alvo '" << id_no << "' nao encontrado no grafo" << endl;
        return {};
    }

    // Construindo um grafo invertido
    // assim facilita pra busca em profundidade achar o caminho indireto
    Grafo *grafo_invertido = new Grafo();
    grafo_invertido->ordem = this->ordem;
    grafo_invertido->in_direcionado = this->in_direcionado;
    grafo_invertido->in_ponderado_aresta = this->in_ponderado_aresta;
    grafo_invertido->in_ponderado_vertice = this->in_ponderado_vertice;

    // Adicionando nos
    for (No *no_original : this->lista_adj)
    {
        No *no_invertido = new No(no_original->id);
        if (this->in_ponderado_vertice)
        {
            no_invertido->peso = no_original->peso;
        }
        else
        {
            no_invertido->peso = 0;
        }
        grafo_invertido->lista_adj.push_back(no_invertido);
        grafo_invertido->mapa_de_nos_por_id[no_original->id] = no_invertido;
    }

    // Adicionando arestas invertidas: aresta u -> v vira  v -> u
    for (No *no_u : this->lista_adj)
    {
        for (Aresta *aresta_original : no_u->arestas)
        {
            char u = no_u->id;
            char v = aresta_original->id_no_alvo;
            int peso = aresta_original->peso;

            No *no_v = nullptr;
            no_v = grafo_invertido->mapa_de_nos_por_id[v];

            no_v->AdicionarVizinho(u, peso);
        }
    }

    // Mapa para controlar os nós visitados
    map<char, bool> visitado;

    // Inicializando todos os nos do grafo como nao visitados
    for (No *no_invertido : grafo_invertido->lista_adj)
    {
        visitado[no_invertido->id] = false;
    }

    // Armazenando os IDs dos nos alcancaveis
    vector<char> nos_alcancaveis;

    // Usando a função auxiliar pra BUSCA EM PROFUNDIDADE a partir do no alvo
    // -> que é o com ID passado por parâmetro
    grafo_invertido->busca_profundidade(grafo_invertido->mapa_de_nos_por_id[id_no], visitado, nos_alcancaveis);

    delete grafo_invertido;

    return nos_alcancaveis;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    // Verifica se os nós de origem e destino existem no grafo -------------------------------------------
    No *no_origem = nullptr;
    No *no_destino = nullptr;
    for (No *no : this->lista_adj)
    {
        if (no->id == id_no_a)
        {
            no_origem = no;
        }
        else if (no->id == id_no_b)
        {
            no_destino = no;
        }
        if (no_origem != nullptr && no_destino != nullptr)
        {
            break;
        }
    }
    if (no_origem == nullptr)
    {
        cout << "Erro: O no de origem '" << id_no_a << "' nao existe no grafo." << endl;
        return {};
    }
    if (no_destino == nullptr)
    {
        cout << "Erro: O no de destino '" << id_no_b << "' nao existe no grafo." << endl;
        return {};
    }
    // ---------------------------------------------------------------------------------------------------

    map<char, int> distancias;     // Armazena as distâncias de cada nó a partir da origem
    map<char, char> predecessores; // Armazena os predecessores de cada nó no caminho mais curto
    map<char, bool> visitados;     // Controle dos nós já visitados {S}

    // Inicializa as distancias como infinito(valor muito alto) para todos os nós.
    int inf = 100000;
    for (No *no : this->lista_adj)
    {
        distancias[no->id] = inf;
        predecessores[no->id] = 0;
        visitados[no->id] = false;
    }

    distancias[id_no_a] = 0; // Define a distância do nó de origem para ele mesmo como 0.

    for (int count = 0; count < this->ordem; ++count)
    {

        int menor_distancia = inf;
        char id_no_j = 0; // Aux para guardar o ID do nó com a menor distância

        for (auto const &par : distancias)
        {
            char id = par.first;
            int dist = par.second;

            if (!visitados[id] && dist < menor_distancia)
            { // Adicionei a verificação !visitados[id] que estava faltando no seu loop de busca manual
                menor_distancia = dist;
                id_no_j = id;
            }
        }
        // Se não encontramos nenhum nó alcançável (id_no_j continua 0), podemos parar.
        if (id_no_j == 0)
        {
            break;
        }

        // Se o nó com menor distância é o nosso destino, podemos parar antes.
        if (id_no_j == id_no_b)
        {
            break;
        }

        visitados[id_no_j] = true;
        No *no_j = nullptr;
        for (No *no : this->lista_adj)
        {
            if (no->id == id_no_j)
            {
                no_j = no;
                break;
            }
        }
        if (no_j == nullptr)
        {
            continue;
        }

        for (Aresta *aresta : no_j->arestas)
        {
            char id_no_i = aresta->id_no_alvo;
            int custo_ji = aresta->peso;
            int distancias_estrela = distancias[id_no_j] + custo_ji;

            if (distancias_estrela < distancias[id_no_i])
            {
                distancias[id_no_i] = distancias_estrela;
                predecessores[id_no_i] = id_no_j;
            }
        }
    }

    // Caminho de volta
    vector<char> caminho;
    char id_atual = id_no_b;

    if (distancias[id_no_b] == inf)
    {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    while (id_atual != 0)
    {
        caminho.push_back(id_atual);
        if (id_atual == id_no_a)
            break;
        id_atual = predecessores[id_atual];
    }

    reverse(caminho.begin(), caminho.end());

    if (caminho.empty() || caminho.front() != id_no_a)
    {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    cout << "Caminho minimo encontrado com custo " << distancias[id_no_b] << endl;
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    // Verifica se os nós de origem e destino existem no grafo -------------------------------------------
    No *no_origem = nullptr;
    No *no_destino = nullptr;
    for (No *no : this->lista_adj)
    {
        if (no->id == id_no_a)
        {
            no_origem = no;
        }
        else if (no->id == id_no_b)
        {
            no_destino = no;
        }
        if (no_origem != nullptr && no_destino != nullptr)
        {
            break;
        }
    }
    if (no_origem == nullptr)
    {
        cout << "Erro: O no de origem '" << id_no_a << "' nao existe no grafo." << endl;
        return {};
    }
    if (no_destino == nullptr)
    {
        cout << "Erro: O no de destino '" << id_no_b << "' nao existe no grafo." << endl;
        return {};
    }
    // ---------------------------------------------------------------------------------------------------

    int inf = 100000; // Um valor grande para representar infinito

    map<char, map<char, int>> dist;  // Matriz Distâncias
    map<char, map<char, char>> pred; // Matriz Predecessores

    // Inicializa a matriz distamância e predecessores.
    for (No *no_i : this->lista_adj)
    {
        for (No *no_j : this->lista_adj)
        {
            char i = no_i->id;
            char j = no_j->id;
            if (i == j)
            {
                dist[i][j] = 0;
            }
            else
            {
                dist[i][j] = inf;
            }
            pred[i][j] = -1;
        }
    }

    for (No *no_atual : this->lista_adj)
    {
        for (Aresta *aresta : no_atual->arestas)
        {
            char u = no_atual->id;
            char v = aresta->id_no_alvo;
            dist[u][v] = aresta->peso;
            pred[u][v] = u;
        }
    }

    for (No *no_k : this->lista_adj)
    {
        char k = no_k->id;
        for (No *no_i : this->lista_adj)
        {
            char i = no_i->id;
            for (No *no_j : this->lista_adj)
            {
                char j = no_j->id;
                if (dist[i][k] != inf && dist[k][j] != inf && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }

    // Caminho de volta
    vector<char> caminho;

    if (dist[id_no_a][id_no_b] == inf)
    {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    char atual = id_no_b;
    while (atual != -1 && pred[id_no_a][atual] != -1)
    {
        caminho.push_back(atual);
        atual = pred[id_no_a][atual];
    }

    if (atual != -1)
        caminho.push_back(atual); // Adiciona o nó de origem, que é o último a ser encontrado

    reverse(caminho.begin(), caminho.end()); // Inverte o caminho, pois ele foi construído do destino para a origem

    // Verificação final para garantir que o caminho é válido
    if (caminho.empty() || caminho.front() != id_no_a)
    {
        cout << "Nao existe caminho entre '" << id_no_a << "' e '" << id_no_b << endl;
        return {};
    }

    cout << "Caminho minimo encontrado com custo " << dist[id_no_a][id_no_b] << endl;
    return caminho;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!in_ponderado_aresta)
    {
        cout << "A arvore de Prim so funciona com grafos ponderados nas arestas" << endl;
        return nullptr;
    }

    unordered_map<char, No *> nos_subgrafo;  // Nós do subconjunto original
    vector<char> ids_indexados;              // IDs do subconjunto em ordem indexada para equivaler a pseudocodigo
    unordered_map<char, int> id_para_indice; // pra usar em prox e custo
    unordered_map<int, char> indice_para_id; // usado ao identificar aresta minima, obtem um indice e precisa do char pra adicionar a arvore

    int indice_atual = 0;
    for (char id : ids_nos)
    {
        No *no_atual = nullptr;

        // Verifica se o no com ID existe no mapa de nós do grafo principal
        if (mapa_de_nos_por_id.count(id))
        {
            no_atual = mapa_de_nos_por_id[id];
        }

        // Adicionando no ao subgrafo e preenchendo mapas e vetor
        if (no_atual != nullptr)
        {
            nos_subgrafo[id] = no_atual;
            ids_indexados.push_back(id);
            id_para_indice[id] = indice_atual;
            indice_para_id[indice_atual] = id;
            indice_atual++;
        }
        else
        {
            cout << "Vertice '" << id << "' no subconjunto X nao encontrado no grafo principal" << endl;
        }
    }

    if (nos_subgrafo.empty())
    {
        cout << "Subconjunto invalido" << endl;
        return nullptr;
    }

    int N = ids_indexados.size();
    // if (N == 0) return nullptr; -> nem precisa por causa da verificação anterior
    if (N == 1)
    { // Arvore = no
        Grafo *agm_um_no = new Grafo();
        agm_um_no->ordem = 1;
        agm_um_no->in_direcionado = false;     // tem q ser pra rodar o Prim
        agm_um_no->in_ponderado_aresta = true; // tem q ser pra rodar o Prim
        agm_um_no->in_ponderado_vertice = this->in_ponderado_vertice;
        agm_um_no->lista_adj.push_back(nos_subgrafo[ids_indexados[0]]);
        agm_um_no->mapa_de_nos_por_id[nos_subgrafo[ids_indexados[0]]->id] = nos_subgrafo[ids_indexados[0]];
        cout << "Peso total da agm: 0" << endl;
        return agm_um_no;
    }

    // Para mais de um no
    Grafo *agm_grafo = new Grafo();
    agm_grafo->ordem = N;
    agm_grafo->in_direcionado = false;
    agm_grafo->in_ponderado_aresta = true;
    agm_grafo->in_ponderado_vertice = this->in_ponderado_vertice;

    unordered_map<char, No *> agm_mapa_nos;
    for (char id : ids_indexados)
    { // pegando os ids dos nos no subgrafo
        agm_grafo->lista_adj.push_back(nos_subgrafo[id]);
        agm_mapa_nos[id] = nos_subgrafo[id];
    }

    vector<int> prox(N);  // Corresponde a 'prox(i)' -> armazena o índice do vértice próximo do vértice i
    vector<int> custo(N); // Corresponde a 'c(i, prox(i))' -> armazena o peso da aresta que conecta o vértice i ao prox[i]

    // Ver se um nó já foi adicionado à AGM
    vector<bool> F(N, false);

    // Definindo no de partida como o primeiro
    char u_id = ids_indexados[0]; // ID do no 'u'

    for (int i = 0; i < N; ++i)
    {
        if (i == 0)
        {
            custo[i] = 0; // Custo zero pra adicionar
            prox[i] = 0;  // colocando ele mesmo como proximo
            F[i] = true;  // Presente na AGM
        }
        else
        {
            custo[i] = numeric_limits<int>::max(); // Custo infinito ->Nao se sabe o caminho mais barato para conectá-los
            prox[i] = -1;                          // indica que ainda não tem proximo
            F[i] = false;
        }
    }

    // A partir do nó de partida, atualiza os nos vizinhos
    No *no_u = nos_subgrafo[u_id];
    for (Aresta *aresta : no_u->arestas)
    {
        char id_vizinho = aresta->id_no_alvo;
        int peso = aresta->peso;

        // Verificando se o vizinho está no subconjunto
        if (nos_subgrafo.count(id_vizinho))
        {
            int indice_vizinho = id_para_indice[id_vizinho];
            // Se o vizinho ainda não está na agm e esta aresta e mais barata que o custo atual
            if (!F[indice_vizinho] && peso < custo[indice_vizinho])
            {
                custo[indice_vizinho] = peso;
                prox[indice_vizinho] = 0; // 'u' é o nó mais próximo
            }
        }
    }

    // Contador para o número de arestas na arvores (n-1 no total)
    int contador = 0;
    int peso_total_arvore = 0;

    // Enquanto contador n-2
    while (contador < N - 1)
    {
        // Seja j tal que prox(j) != 0 e c(j,prox(j)) é mínimo.
        int j_indice = -1;
        int min_custo = numeric_limits<int>::max(); // custo infinito

        for (int i = 0; i < N; ++i)
        {
            // Nó i ainda não está na arvore e tem custo minimo
            if (!F[i] && custo[i] < min_custo)
            {
                min_custo = custo[i];
                j_indice = i;
            }
        }

        if (j_indice == -1)
        {
            // Sem no pra conectar
            cout << "AGM incompleta ou impossivel." << endl;
            break;
        }

        // Marca j como adicionado a arvore
        F[j_indice] = true;
        peso_total_arvore += custo[j_indice]; // Adiciona o peso a arvore

        // Adiciona a aresta a arvore
        // A aresta é entre j e o proximo de j
        char no_j_id = indice_para_id[j_indice];
        char no_prox_j_id = indice_para_id[prox[j_indice]];

        No *no_j = agm_mapa_nos[no_j_id];
        No *no_j_prox = agm_mapa_nos[no_prox_j_id];

        // Adicionar aresta nos dois sentidos
        no_j->AdicionarVizinho(no_j_prox->id, custo[j_indice]);
        no_j_prox->AdicionarVizinho(no_j->id, custo[j_indice]);

        contador++;

        // Para i = 1,...,n faça
        //   Se prox(i) ≠ 0 e c(i,prox(i)) > c(i,j) então
        //     prox(i) <- j
        // fim-para

        No *no_j_sub = nos_subgrafo[no_j_id];
        for (Aresta *aresta_j : no_j_sub->arestas)
        { // Vizinhos de j
            char id_vizinho = aresta_j->id_no_alvo;

            // Verifica se o vizinho está no subconjunto e ainda não está na arvoree
            if (nos_subgrafo.count(id_vizinho) && !F[id_para_indice[id_vizinho]])
            {
                int indice_vizinho = id_para_indice[id_vizinho];
                // Ver se a aresta (j -> neighbor) é mais barata que a aresta atual mais barata para 'neighbor'
                if (aresta_j->peso < custo[indice_vizinho])
                {
                    custo[indice_vizinho] = aresta_j->peso;
                    prox[indice_vizinho] = j_indice; // 'j' é agora o nó mais próximo
                }
            }
        }
    }

    // Verificação final de conectividade
    if (N > 1 && contador != N - 1)
    {
        cout << "O subgrafo vertice-induzido por X nao e conectado" << endl;
    }

    cout << "Peso total da arvore geradora minima: " << peso_total_arvore << endl;

    return agm_grafo;
}

// Funções auxiliares para a Arvore Geradora Mínima Kruskal
char encontrar_raiz(char no, std::map<char, char> &pai)
{
    if (pai[no] == no)
    {
        return no;
    }
    pai[no] = encontrar_raiz(pai[no], pai);
    return pai[no];
}

void unir_conjuntos(char u, char v, std::map<char, char> &pai)
{
    char raiz_u = encontrar_raiz(u, pai);
    char raiz_v = encontrar_raiz(v, pai);
    if (raiz_u != raiz_v)
    {
        pai[raiz_u] = raiz_v;
    }
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    if (ids_nos.empty())
    {
        cout << "Erro: O subconjunto de nos nao pode ser vazio." << endl;
        return nullptr;
    }

    std::map<int, std::vector<std::pair<char, char>>> arestas_por_peso;
    std::map<char, std::map<char, bool>> aresta_adicionada;

    for (char id_no : ids_nos)
    {
        No *no_atual = mapa_de_nos_por_id[id_no];
        for (Aresta *aresta : no_atual->arestas)
        {
            bool vizinho_no_subgrafo = false;
            for (char id_vizinho_verificar : ids_nos)
            {
                if (aresta->id_no_alvo == id_vizinho_verificar)
                {
                    vizinho_no_subgrafo = true;
                    break;
                }
            }

            if (vizinho_no_subgrafo)
            {
                char u = id_no;
                char v = aresta->id_no_alvo;

                if (u > v)
                {
                    char temp = u;
                    u = v;
                    v = temp;
                }

                if (!aresta_adicionada[u][v])
                {
                    arestas_por_peso[aresta->peso].push_back({u, v});
                    aresta_adicionada[u][v] = true;
                }
            }
        }
    }

    std::map<char, char> pai;
    for (char id : ids_nos)
    {
        pai[id] = id;
    }

    Grafo *agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->ordem = ids_nos.size();
    for (char id_no : ids_nos)
    {
        No *no_novo = new No(id_no);
        agm->lista_adj.push_back(no_novo);
        agm->mapa_de_nos_por_id[id_no] = no_novo;
    }

    int num_arestas_agm = 0;
    bool arvore_completa = false;

    for (auto const &par_peso : arestas_por_peso)
    {
        int peso = par_peso.first;
        for (auto const &par_nos : par_peso.second)
        {
            char u = par_nos.first;
            char v = par_nos.second;

            if (encontrar_raiz(u, pai) != encontrar_raiz(v, pai))
            {
                unir_conjuntos(u, v, pai);
                agm->mapa_de_nos_por_id[u]->AdicionarVizinho(v, peso);
                agm->mapa_de_nos_por_id[v]->AdicionarVizinho(u, peso);
                num_arestas_agm++;

                if (num_arestas_agm == ids_nos.size() - 1)
                {
                    arvore_completa = true;
                    break;
                }
            }
        }
        if (arvore_completa)
        {
            break;
        }
    }

    if (num_arestas_agm < ids_nos.size() - 1)
    {
        cout << "Aviso: Nao foi possivel gerar uma arvore conectada com os nos fornecidos." << endl
             << endl;
    }

    return agm;
}

void Grafo::aux_camainhamento_profundidade(char id_atual, map<char, bool> &visitados, vector<char> &nos_da_arvore, vector<pair<char, char>> &arestas_da_arvore)
{

    // Marca o nó atual como visitado e o adiciona à lista de nós da árvore
    visitados[id_atual] = true;
    nos_da_arvore.push_back(id_atual);

    No *no_atual = mapa_de_nos_por_id[id_atual];

    for (Aresta *aresta : no_atual->arestas)
    {
        char id_vizinho = aresta->id_no_alvo;

        if (!visitados[id_vizinho])
        {
            arestas_da_arvore.push_back({id_atual, id_vizinho});
            aux_camainhamento_profundidade(id_vizinho, visitados, nos_da_arvore, arestas_da_arvore);
        }
    }
}
Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    // Verifica se o nó de início existe
    if (mapa_de_nos_por_id.find(id_no) == mapa_de_nos_por_id.end())
    {
        cout << "Erro: O no de inicio '" << id_no << "' nao existe no grafo." << endl;
        return nullptr;
    }

    map<char, bool> visitados;
    for (auto const &par : mapa_de_nos_por_id)
    {
        visitados[par.first] = false;
    }

    vector<char> nos_da_arvore;
    vector<pair<char, char>> arestas_da_arvore;

    aux_camainhamento_profundidade(id_no, visitados, nos_da_arvore, arestas_da_arvore);

    Grafo *arvore_profundidade = new Grafo();
    arvore_profundidade->in_direcionado = this->in_direcionado;
    arvore_profundidade->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore_profundidade->ordem = nos_da_arvore.size();

    for (char id_no : nos_da_arvore)
    {
        int peso_no = mapa_de_nos_por_id[id_no]->peso;
        No *no_novo = new No(id_no);
        no_novo->peso = peso_no;

        arvore_profundidade->lista_adj.push_back(no_novo);
        arvore_profundidade->mapa_de_nos_por_id[id_no] = no_novo;
    }

    for (const auto &par_aresta : arestas_da_arvore)
    {
        char u = par_aresta.first;
        char v = par_aresta.second;

        int peso_aresta = 1;
        if (this->in_ponderado_aresta)
        {
            for (Aresta *aresta_original : mapa_de_nos_por_id[u]->arestas)
            {
                if (aresta_original->id_no_alvo == v)
                {
                    peso_aresta = aresta_original->peso;
                    break;
                }
            }
        }

        arvore_profundidade->mapa_de_nos_por_id[u]->AdicionarVizinho(v, peso_aresta);
        if (!arvore_profundidade->in_direcionado)
        {
            arvore_profundidade->mapa_de_nos_por_id[v]->AdicionarVizinho(u, peso_aresta);
        }
    }

    cout << "Arvore de caminhamento em profundidade gerada a partir do no '" << id_no << endl;
    return arvore_profundidade;
}

int Grafo::raio()
{
    int raio_grafo = this->ordem; // raio do grafo
    int excentricidade_no = 0;    // excentricidade de um nó
    int aux_size;                 // guarda o tamanho de uma iteração de Dijstra
    // loop duplo para calcular o caminho mínimo entre 2 nós distintos, para todos os pares de nós
    for (auto const &nos1 : mapa_de_nos_por_id)
    {
        for (auto const &nos2 : mapa_de_nos_por_id)
        {
            if (nos1 != nos2)
            {
                // numero arestas = caminho - 1
                aux_size = (int)caminho_minimo_dijkstra(nos1.first, nos2.first).size() - 1;
                excentricidade_no = max(aux_size, excentricidade_no);
            }
        }
        // avaliando se a excentricidade é mínima
        raio_grafo = min(excentricidade_no, raio_grafo);
    }
    return raio_grafo;
}

int Grafo::diametro()
{
    int diametro_grafo = 0;    // diametro do grafo
    int excentricidade_no = 0; // excentricidade de um nó
    int aux_size;              // guarda o tamanho de uma iteração de Dijstra
    // loop duplo para calcular o caminho mínimo entre 2 nós distintos, para todos os pares de nós
    for (auto const &nos1 : mapa_de_nos_por_id)
    {
        for (auto const &nos2 : mapa_de_nos_por_id)
        {
            if (nos1 != nos2)
            {
                // numero arestas = caminho - 1
                aux_size = caminho_minimo_dijkstra(nos1.first, nos2.first).size() - 1;
                excentricidade_no = max(aux_size, excentricidade_no);
            }
        }
        // avaliando se a excentricidade é máxima
        diametro_grafo = max(excentricidade_no, diametro_grafo);
    }
    return diametro_grafo;
}

vector<char> Grafo::centro()
{
    int raio_grafo = raio();   // raio do grafo
    vector<char> centro_nos;   // vértices centrais
    int excentricidade_no = 0; // excentricidade de um nó
    int aux_size;              // guarda o tamanho de uma iteração de Dijstra
    // loop duplo para calcular o caminho mínimo entre 2 nós distintos, para todos os pares de nós
    for (auto const &nos1 : mapa_de_nos_por_id)
    {
        for (auto const &nos2 : mapa_de_nos_por_id)
        {
            if (nos1 != nos2)
            {
                // numero arestas = caminho - 1
                aux_size = (int)caminho_minimo_dijkstra(nos1.first, nos2.first).size() - 1;
                excentricidade_no = max(aux_size, excentricidade_no);
            }
        }
        // se vértice for central, é salvo no vetor
        if (excentricidade_no == raio_grafo)
        {
            centro_nos.push_back(nos1.first);
        }
    }
    return centro_nos;
}

vector<char> Grafo::periferia()
{
    int diametro_grafo = diametro(); // diametro do grafo
    vector<char> periferia_nos;      // vértices periféricos
    int excentricidade_no = 0;       // excentricidade de um nó
    int aux_size;                    // guarda o tamanho de uma iteração de Dijstra
    // loop duplo para calcular o caminho mínimo entre 2 nós distintos, para todos os pares de nós
    for (auto const &nos1 : mapa_de_nos_por_id)
    {
        for (auto const &nos2 : mapa_de_nos_por_id)
        {
            if (nos1 != nos2)
            {
                // numero arestas = caminho - 1
                aux_size = (int)caminho_minimo_dijkstra(nos1.first, nos2.first).size() - 1;
                excentricidade_no = max(aux_size, excentricidade_no);
            }
        }
        // se vértice for periférico, é salvo no vetor
        if (excentricidade_no == diametro_grafo)
        {
            periferia_nos.push_back(nos1.first);
        }
    }
    return periferia_nos;
}

/*void Grafo::imprimirGrafo() {
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
}*/
