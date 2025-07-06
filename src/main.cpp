#include <iostream>
#include "Gerenciador.h"

using namespace std;
int main(int argc, char *argv[])
{
    Grafo* grafo = new Grafo(); 

    // Caso nao seja passado o arquivo txt
    if (!grafo->ler_arquivo_entrada(argv[1])) { 
        cout << "Nao foi possivel carregar o grafo do arquivo" << endl;
        delete grafo; 
        return 1; 
    }

    cout << "Carregando grafo do arquivo: " << argv[1] << endl;

    grafo->imprimirGrafo();

    Gerenciador::comandos(grafo); 

    delete grafo; 
    return 0;
}
