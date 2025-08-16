# T2 - DCC059 - Teoria dos Grafos (2025/1)

Repositório do Trabalho 2 da disciplina DCC059 (Teoria dos Grafos), ministrada pela Profª. Luciana Brugiolo na Universidade Federal de Juiz de Fora (UFJF).

O objetivo deste trabalho é a implementação de algoritmos heurísticos para o problema do **Conjunto Dominante Conectado** (*Connected Dominating Set*). Foram implementados os seguintes algoritmos:
* Algoritmo Guloso
* Algoritmo Guloso Randomizado Adaptativo (GRASP)
* Algoritmo Guloso Randomizado Adaptativo Reativo

## Pré-requisitos

Para compilar e executar o projeto, é necessário ter um compilador C++ instalado, preferencialmente o **g++**, que faz parte do GNU Compiler Collection (GCC).

## Compilação

Todo o código-fonte se encontra na pasta `src`. Para compilar o projeto, siga os passos:

1.  Navegue até o diretório do código-fonte:
    ```bash
    cd src
    ```

2.  Compile os arquivos `.cpp` para gerar o executável (ex: `execGrupo`):
    ```bash
    g++ *.cpp -o execGrupo
    ```
    Isso criará o arquivo `execGrupo` dentro da pasta `src`.

## Execução

**Importante:** O programa deve ser executado de **dentro da pasta `src`** e o caminho para o arquivo de instância deve ser passado como um **argumento de linha de comando**.

1.  Para executar o programa, utilize o seguinte formato:
    ```bash
    ./execGrupo <caminho_para_instancia>
    ```

2.  Como o executável está em `src`, você deve usar `../` para "subir" um nível no diretório e acessar a pasta `instancias_t2`.

    **Exemplo prático de execução:**
    ```bash
    ./execGrupo ../instancias_t2/g_25_0.16_0_1_0.txt
    ```

3.  Após carregar o grafo, o programa exibirá um menu. Para executar os algoritmos deste trabalho, utilize as seguintes opções:

    * **`(i)` - Algoritmo Guloso:** Executa a versão determinística.
    * **`(j)` - Algoritmo GRASP:** Solicitará o número de iterações e um valor de alfa.
    * **`(k)` - Algoritmo Reativo:** Solicitará o número de iterações e usará o conjunto de alfas pré-definido no código (`{0.1, 0.2, 0.3, 0.4, 0.5}`).
