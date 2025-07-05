#ifndef ARESTA_H
#define ARESTA_H

using namespace std;

class Aresta {
public:
    Aresta(char id_alvo, int p); 
    ~Aresta();

    char id_no_alvo;
    int peso;
};

#endif //ARESTA_H
