#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar uma aresta
typedef struct {
    int origem, destino, peso;
} Aresta;

// Estrutura para representar um subconjunto (usado na união-find)
typedef struct {
    int pai;
    int rank;
} Subconjunto;

// Função para comparar arestas pelo peso (para usar no qsort)
int comparaArestas(const void *a, const void *b) {
    Aresta *A = (Aresta *)a;
    Aresta *B = (Aresta *)b;
    return A->peso - B->peso;
}

// Função para encontrar o conjunto de um elemento (com compressão de caminho)
int encontrar(Subconjunto subconj[], int i) {
    if (subconj[i].pai != i)
        subconj[i].pai = encontrar(subconj, subconj[i].pai);
    return subconj[i].pai;
}

// Função para unir dois subconjuntos
void unir(Subconjunto subconj[], int x, int y) {
    int raizX = encontrar(subconj, x);
    int raizY = encontrar(subconj, y);

    if (subconj[raizX].rank < subconj[raizY].rank)
        subconj[raizX].pai = raizY;
    else if (subconj[raizX].rank > subconj[raizY].rank)
        subconj[raizY].pai = raizX;
    else {
        subconj[raizY].pai = raizX;
        subconj[raizX].rank++;
    }
}

int main() {
    FILE *arquivo = fopen("in.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo in.txt\n");
        return 1;
    }

    int V, E;
    fscanf(arquivo, "%d %d", &V, &E);

    Aresta *arestas = (Aresta *)malloc(E * sizeof(Aresta));

    for (int i = 0; i < E; i++) {
        fscanf(arquivo, "%d %d %d", &arestas[i].origem, &arestas[i].destino, &arestas[i].peso);
    }
    fclose(arquivo);

    // Ordena as arestas pelo peso
    qsort(arestas, E, sizeof(Aresta), comparaArestas);

    // Cria subconjuntos (um para cada vértice)
    Subconjunto *subconj = (Subconjunto *)malloc((V + 1) * sizeof(Subconjunto));
    for (int v = 1; v <= V; v++) {
        subconj[v].pai = v;
        subconj[v].rank = 0;
    }

    printf("Arestas escolhidas na Árvore Geradora Mínima (MST):\n");

    int arestasEscolhidas = 0;
    int i = 0;
    int custoTotal = 0;

    // Kruskal: percorre as arestas em ordem crescente
    while (arestasEscolhidas < V - 1 && i < E) {
        Aresta proxima = arestas[i++];

        int x = encontrar(subconj, proxima.origem);
        int y = encontrar(subconj, proxima.destino);

        // Se não forma ciclo, adiciona à MST
        if (x != y) {
            printf("(%d -- %d) peso: %d\n", proxima.origem, proxima.destino, proxima.peso);
            custoTotal += proxima.peso;
            unir(subconj, x, y);
            arestasEscolhidas++;
        }
    }

    printf("Custo total da MST = %d\n", custoTotal);

    free(arestas);
    free(subconj);
    return 0;
}