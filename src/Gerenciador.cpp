#include "Gerenciador.h"
#include <fstream>
#include <ostream>

void Gerenciador::comandos(Grafo *grafo)
{
    cout << "Digite uma das opcoes abaixo e pressione enter:" << endl
         << endl;
    cout << "(a) Fecho transitivo direto de um no;" << endl;
    cout << "(b) Fecho transitivo indireto de um no;" << endl;
    cout << "(c) Caminho minimo (Djikstra);" << endl;
    cout << "(d) Caminho minimo (Floyd);" << endl;
    cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
    cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
    cout << "(g) Arvore de caminhamento em profundidade;" << endl;
    cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
    cout << "(0) Sair;" << endl
         << endl;

    char resp;
    cin >> resp;
    switch (resp)
    {
    case 'a':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
        imprimir_vetor_char(fecho_transitivo_direto, cout);

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            ofstream outfile("fecho_trans_dir.txt");
            if (outfile.is_open())
            {
                imprimir_vetor_char(fecho_transitivo_direto, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }

        break;
    }

    case 'b':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
        imprimir_vetor_char(fecho_transitivo_indireto, cout);

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            ofstream outfile("fecho_trans_indir.txt");
            if (outfile.is_open())
            {
                imprimir_vetor_char(fecho_transitivo_indireto, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }

        break;
    }

    case 'c':
    {

        cout << "Digite primeiro a origem e pressione ENTER e apos digite o destino" << endl;
        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);
        imprimir_vetor_char(caminho_minimo_dijkstra, cout);

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            ofstream outfile("caminho_minimo_dijkstra.txt");
            if (outfile.is_open())
            {
                imprimir_vetor_char(caminho_minimo_dijkstra, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }
        break;
    }

    case 'd':
    {

        cout << "Digite primeiro a origem e pressione ENTER e apos digite o destino" << endl;
        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
        imprimir_vetor_char(caminho_minimo_floyd, cout);

        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            ofstream outfile("caminho_minimo_floyd.txt");
            if (outfile.is_open())
            {
                imprimir_vetor_char(caminho_minimo_floyd, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }

        break;
    }
    case 'e':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
            imprimir_grafo_adj(arvore_geradora_minima_prim, cout);

            if (pergunta_imprimir_arquivo("agm_prim.txt"))
            {
                ofstream outfile("agm_prim.txt");
                if (outfile.is_open())
                {
                    imprimir_grafo_adj(arvore_geradora_minima_prim, outfile);
                    outfile.close();
                    cout << "Resultado salvo com sucesso." << endl
                         << endl;
                }
                else
                {
                    cout << "Erro ao abrir arquivo para escrita." << endl
                         << endl;
                }
            }

            delete arvore_geradora_minima_prim;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'f':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
            imprimir_grafo_adj(arvore_geradora_minima_kruskal, cout);

            if (pergunta_imprimir_arquivo("agm_kruskal.txt"))
            {
                ofstream outfile("agm_kruskal.txt");
                if (outfile.is_open())
                {
                    imprimir_grafo_adj(arvore_geradora_minima_kruskal, outfile);
                    outfile.close();
                    cout << "Resultado salvo com sucesso." << endl
                         << endl;
                }
                else
                {
                    cout << "Erro ao abrir arquivo para escrita." << endl
                         << endl;
                }
            }

            delete arvore_geradora_minima_kruskal;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'g':
    {

        char id_no = get_id_entrada();
        Grafo *arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
        imprimir_grafo_adj(arvore_caminhamento_profundidade, cout);

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
        {
            ofstream outfile("arvore_caminhamento_profundidade.txt");
            if (outfile.is_open())
            {
                imprimir_grafo_adj(arvore_caminhamento_profundidade, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }

        delete arvore_caminhamento_profundidade;
        break;
    }

    case 'h':
    {
        int raio = grafo->raio();
        int diametro = grafo->diametro();
        vector<char> centro = grafo->centro();
        vector<char> periferia = grafo->periferia();

        cout << endl;
        cout << "Raio do grafo: " << raio << endl;
        cout << "Diametro do grafo: " << diametro << endl;
        cout << "Vértices do centro: ";
        for (auto i : centro)
        {
            cout << i << " ";
        }
        cout << endl;
        cout << "Vértices da periferia: ";
        for (auto i : periferia)
        {
            cout << i << " ";
        }
        cout << endl
             << endl;

        if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt"))
        {
            ofstream outfile("raio_diametro_centro_periferia.txt");
            if (outfile.is_open())
            {
                imprimir_inteiro(raio, outfile);
                imprimir_inteiro(diametro, outfile);
                imprimir_vetor_char(centro, outfile);
                imprimir_vetor_char(periferia, outfile);
                outfile.close();
                cout << "Resultado salvo com sucesso." << endl
                     << endl;
            }
            else
            {
                cout << "Erro ao abrir arquivo para escrita." << endl
                     << endl;
            }
        }

        break;
    }

    case '0':
    {
        exit(0);
    }
    default:
    {
        cout << "Opção inválida" << endl;
    }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada()
{
    cout << "Digite o id de um no: ";
    char id;
    cin >> id;
    cout << endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam)
{
    vector<char> ids = {};
    while ((int)ids.size() < tam)
    {
        char id_no = get_id_entrada();
        bool existe = false;
        for (No *no : grafo->lista_adj)
        {
            if (no->id == id_no)
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            cout << "Vertice nao existe" << endl;
        }
        else
        {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido)
            {
                cout << "Valor repetido" << endl;
            }
            else
            {
                ids.push_back(id_no);
            }
        }
    }

    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo)
{

    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;
    int resp;
    cin >> resp;
    cout << endl;

    switch (resp)
    {
    case 1:
        return true;
    case 2:
        return false;
    default:
        cout << "Resposta invalida" << endl;
        return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

// Imprime um vetor de char no formato: a,b,c
void Gerenciador::imprimir_vetor_char(const vector<char> &vetor, ostream &saida)
{
    for (size_t i = 0; i < vetor.size(); ++i)
    {
        saida << vetor[i];
        if (i < vetor.size() - 1)
        {
            saida << ",";
        }
    }
    saida << endl;
}

// Imprime um inteiro
void Gerenciador::imprimir_inteiro(const int &num, ostream &saida)
{
    saida << num;
    saida << endl;
}

// Imprime um grafo no formato de lista de adjacências: a: b -> c
void Gerenciador::imprimir_grafo_adj(Grafo *grafo, ostream &saida)
{
    for (No *no : grafo->lista_adj)
    {
        saida << no->id << ": ";
        for (size_t i = 0; i < no->arestas.size(); ++i)
        {
            saida << no->arestas[i]->id_no_alvo;
            if (i < no->arestas.size() - 1)
            {
                saida << " -> ";
            }
        }
        saida << endl;
    }
}