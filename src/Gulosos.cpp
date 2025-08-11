#include "Gulosos.h"
#include <map>
#include <random>
#include <algorithm>
#include <numeric>

const int Gulosos::BRANCO; // REFERENTE AOS NÓS NÃO DOMINADOS
const int Gulosos::CINZA; // REFERENTE AOSS NÓS DOMINADOS
const int Gulosos::PRETO; // REFERENTE AOS NOS NA SOLUÇÃO

Gulosos::Gulosos(Grafo* grafo) : grafo(grafo) {}

vector<char> Gulosos::guloso() {
    //USANDO SET PRA OTIMIZAR - NAO PERMITE REPETIÇÃO E MANTEM ORDEENADO
    set<char> D = construir_conjunto_dominante(0.0); // CHAMA A FUNÇÃO COM ALFA = 0 -> SEM ALEATORIEDADE
    conectar_componentes(D);                         
    
    return vector<char>(D.begin(), D.end()); //PASSANDO PRA VECTOR PRA TER 
                                             //COMPATIBILIDADE COM O RESTO DO CODIGO
}

//RECEBE O NUMERO DE ITERAÇÕES E O ALFA DESEJADOS 
vector<char> Gulosos::g_rand_adapt(int max_iteracoes, double alfa) {
    set<char> melhor_D;
    size_t melhor_tamanho = grafo->ordem + 1;

    for (int i = 0; i < max_iteracoes; ++i) {
        //CONSTROI O CONJUNTO PARA UM DETERMINADO ALFA E APÓS CONECTA O CONJUNTO D 
        set<char> D_candidato = construir_conjunto_dominante(alfa);
        conectar_componentes(D_candidato);

        //VERIFICA SE O NOVO CANDIDATO TEM O MENOR TAMANHO ATE O MOMENTO 
        // E ATUALIZA O DOMIANTE QUNATO A VERTICE E TAMANHO
        if (D_candidato.size() < melhor_tamanho) {
            melhor_D = D_candidato;
            melhor_tamanho = D_candidato.size();
        }
    }
    return vector<char>(melhor_D.begin(), melhor_D.end());
}

//RECEBE O NUMERO DE ITERAÇÕES DESEJADAS E ALFAS PREDEFINIDOS 
vector<char> Gulosos::g_rand_adapt_reativo(int max_iteracoes, const vector<double>& alfas) {
    if (alfas.empty()) {
        cout << "O vetor de alfas para o Guloso Randomizado Adaptativo Reativo nao pode ser vazio." << endl;
        return {};
    }

    set<char> melhor_D;
    size_t melhor_tamanho = grafo->ordem + 1;

    //PONTUACOES_ALFA SERVE PRA GUARDAR OS ALFAS E RALACIONA-LOS COM A SOMA 
    // DOS TAMANHOS DAS SOLUCOES QUE GEROU
    map<double, double> pontuacoes_alfa;

    // CONTAGENS_ALFA GUARDA QUANTAS VEZES O ALFA FOI USADO,
    // PRA SER USADO COM PONTUACOES PRA VER O DESEMPENHO DO ALFA -> PONTUACAO/CONTAGEM
    map<double, int> contagens_alfa;

    //PROBABILIDADES_ALFA TEM TODOS ALFAS, COM CADA ALFA TENHO UMA PROBABILIDADE BASEADA EM SEU DESEMPENHO
    vector<double> probabilidades_alfa(alfas.size(), 1.0 / alfas.size());

    //SEED PRA GERAR NUMEROS ALEATORIOS
    random_device rd;
    mt19937 gen(rd());

    for (int i = 0; i < max_iteracoes; ++i) {
        //DISTRIBUICAO DE VALORES DE ALFA PONDERADOS PELAS PROBABILIDADES
        discrete_distribution<> dist(probabilidades_alfa.begin(), probabilidades_alfa.end());
        //AQUI É FEITA A ESCOLHA DE ALFA ALEATORIAMENTE DENTRE OS VALORES DA DISTRIBUICAO
        double alfa_atual = alfas[dist(gen)];

        //BUSCANDO SOLUCAO PARA O ALFA SELECIONADO
        set<char> D_candidato = construir_conjunto_dominante(alfa_atual);
        conectar_componentes(D_candidato);

        //VERIFICANDO SE É A MELHOR SOLUCAO
        if (D_candidato.size() < melhor_tamanho) {
            melhor_D = D_candidato;
            melhor_tamanho = D_candidato.size();
        }

        //GUARDANDO O DESEMPENHO DE ALFA
        pontuacoes_alfa[alfa_atual] += D_candidato.size();
        contagens_alfa[alfa_atual]++;

        // COLOCANDO O ALGORITMO PRA REVISAR OS ALFAS A CADA 5 ITERACOES
        if (i > 0 && i % 5 == 0) {
            double soma_inversos_media = 0;
            vector<double> q(alfas.size(), 0.0);

            for (size_t j = 0; j < alfas.size(); ++j) {
                double alfa = alfas[j];
                if (contagens_alfa.count(alfa) && contagens_alfa[alfa] > 0) {
                    double media = pontuacoes_alfa[alfa] / contagens_alfa[alfa];
                    q[j] = 1.0 / media; // Inverso da média (soluções menores são melhores)
                    soma_inversos_media += q[j];
                }
            }

            if (soma_inversos_media > 0) {
                for (size_t j = 0; j < alfas.size(); ++j) {
                    probabilidades_alfa[j] = q[j] / soma_inversos_media;
                }
            }
        }
    }
    return vector<char>(melhor_D.begin(), melhor_D.end());
}


set<char> Gulosos::construir_conjunto_dominante(double alfa) {
    set<char> conjunto_dominante;
    map<char, int> status;
    int nos_brancos = grafo->ordem;

    for (auto const& par : grafo->mapa_de_nos_por_id) {
        status[par.first] = BRANCO;
    }

    random_device rd;
    mt19937 gen(rd());

    while (nos_brancos > 0) {
        map<char, int> poder_dominancia;
        int poder_maximo = 0;

        for (auto const& par : grafo->mapa_de_nos_por_id) {
            char id_no = par.first;
            if (conjunto_dominante.find(id_no) == conjunto_dominante.end()) {
                int poder = 0;
                if (status[id_no] == BRANCO) {
                    poder++;
                }
                for (Aresta* aresta : par.second->arestas) {
                    if (status[aresta->id_no_alvo] == BRANCO) {
                        poder++;
                    }
                }
                poder_dominancia[id_no] = poder;
                if (poder > poder_maximo) {
                    poder_maximo = poder;
                }
            }
        }

        vector<char> rcl; // Lista de Candidatos Restrita
        double limiar = static_cast<double>(poder_maximo) * (1.0 - alfa);
        for (auto const& par : poder_dominancia) {
            if (static_cast<double>(par.second) >= limiar) {
                rcl.push_back(par.first);
            }
        }

        if (rcl.empty()) break;

        uniform_int_distribution<> dist(0, rcl.size() - 1);
        char id_escolhido = rcl[dist(gen)];
        conjunto_dominante.insert(id_escolhido);

        if (status[id_escolhido] == BRANCO) {
            nos_brancos--;
        }
        status[id_escolhido] = PRETO;

        No* no_escolhido = grafo->mapa_de_nos_por_id[id_escolhido];
        for (Aresta* aresta : no_escolhido->arestas) {
            if (status[aresta->id_no_alvo] == BRANCO) {
                status[aresta->id_no_alvo] = CINZA;
                nos_brancos--;
            }
        }
    }
    return conjunto_dominante;
}

void Gulosos::conectar_componentes(set<char>& conjunto_dominante) {
    while (!eh_conectado(conjunto_dominante)) {
        bool no_adicionado = false;
        for (char id_u : conjunto_dominante) {
            No* no_u = grafo->mapa_de_nos_por_id[id_u];
            for (Aresta* aresta : no_u->arestas) {
                char id_v = aresta->id_no_alvo;
                if (conjunto_dominante.find(id_v) == conjunto_dominante.end()) {
                    conjunto_dominante.insert(id_v);
                    no_adicionado = true;
                    goto proxima_iteracao;
                }
            }
        }
        proxima_iteracao:;
        if (!no_adicionado) break;
    }
}

bool Gulosos::eh_conectado(const set<char>& nos) {
    if (nos.empty() || nos.size() == 1) {
        return true;
    }
    set<char> visitados;
    char id_inicio = *nos.begin();
    dfs_subgrafo(id_inicio, nos, visitados);
    return visitados.size() == nos.size();
}

void Gulosos::dfs_subgrafo(char id_no, const set<char>& nos, set<char>& visitados) {
    visitados.insert(id_no);
    No* no_u = grafo->mapa_de_nos_por_id[id_no];

    for (Aresta* aresta : no_u->arestas) {
        char id_v = aresta->id_no_alvo;
        if (nos.count(id_v) && visitados.find(id_v) == visitados.end()) {
            dfs_subgrafo(id_v, nos, visitados);
        }
    }
}
