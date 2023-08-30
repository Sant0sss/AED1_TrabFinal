#include <stdio.h>
#include <stdlib.h>


typedef struct t_celula{
    struct t_celula *right, *below;
    int line, columns;
    float valor;
}tipo_celula;

typedef struct{
    int m, n;
    tipo_celula *inicio, *fimLine, *fimColumns;
}Matrix;

typedef Matrix Matrix;


//funcoes
Matrix *matrix_create(int m, int n);
int iniciaCabecas(Matrix *mat);
int insereCabecaLinha(Matrix *mat);
int insereCabecaColuna(Matrix *mat);

int matrix_setelem(Matrix *mat, int linha, int coluna, float valor);
int matrix_print(Matrix *mat);
int matrix_destroy(Matrix *mat);
Matrix *matrix_add(Matrix *matA, Matrix *matB);
Matrix *matrix_multiply(Matrix *matA, Matrix *matB);

float matrix_getelem(Matrix *mat, int linha, int coluna);

//m = linhas
//n = colunas

//funcoes
Matrix *matrix_create(int m, int n){
    Matrix *mat;
	mat = (Matrix *)malloc(sizeof(Matrix));

	if (!mat || m<=0 || n<=0){
        return 0;
	}

	//inicia as variaveis da struct
	mat->inicio = NULL;
	mat->fimLine = NULL;
	mat->fimColumns = NULL;
	mat->m = m;
	mat->n = n;

	iniciaCabecas(mat);

	return mat;
}

int iniciaCabecas(Matrix *mat){ //cria a estrutura
    int i=0;

    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0; //erro
    }

    //celula principal
    cabeca->columns = -1;
    cabeca->line = -1;

    mat->inicio = cabeca;
    mat->fimLine = cabeca;
    mat->fimColumns = cabeca;

    for (i = 1; i <= mat->n; i++){ //cabecas colunas, | | | |
        insereCabecaColuna(mat);
    }
    for (i = 1; i <= mat->m; i++){ //cabecas linhas
        insereCabecaLinha(mat);
    }
    return 1;
}
int insereCabecaColuna(Matrix *mat){ //cabecas colunas, | | | |
    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0; //erro
    }

    cabeca->columns = -1;
    cabeca->line = 0;

    mat->fimColumns->right = cabeca;
    mat->fimColumns = cabeca;

    cabeca->right = mat->inicio;
    cabeca->below = cabeca;
    return 1;
}

int insereCabecaLinha(Matrix *mat){
    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0;
    }

    cabeca->columns = 0;
    cabeca->line = -1;

    mat->fimLine->below = cabeca;
    mat->fimLine = cabeca;

    cabeca->below = mat->inicio;
    cabeca->right = cabeca;

    return 1;
}

int matrix_setelem(Matrix *mat, int line, int columns, float valor){
    int i;

    if (!mat || mat->m <= 0 || mat->n <= 0 || !valor){ //esses testes sao feitos tbm na funcao lerMatriz
        return 0;
    }
    if (line>mat->m || columns>mat->n || !valor || line < 1 || columns < 1){
        return 0;
    }

    tipo_celula *pCelula; //celula temporaria
    tipo_celula *pCelulaColumns; //celula temporaria para coluna
    tipo_celula *pCelulaLine; //celula temporaria para linha

    pCelula = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!pCelula){ //memoria cheia
        return 0;
    }

    pCelula->line = line;
    pCelula->columns = columns;
    pCelula->valor = valor;

    pCelulaLine = mat->inicio->below; //linha aponta para a primeira cabeca so de linha
    pCelulaColumns = mat->inicio->right; //linha aponta para a primeira cabeca so de coluna

    //Percorre ate a linha do elemento, percorre nas cabecas ate a informada
    for (i=0; i<line-1; i++){ //-1 pq ja ta na primeira
        pCelulaLine = pCelulaLine->below; //percorre para baixo(linhas) ate achar a correta
    }
    i=0;
    while (i<columns && pCelulaLine->right->line != -1){
        if (pCelulaLine->right->columns > pCelula->columns){ //se a celula inserida esta a esquerda
            pCelula->right = pCelulaLine->right;
            pCelulaLine->right = pCelula;
        }
        else{
            pCelulaLine = pCelulaLine->right; //anda pelas celulas a direita
        }
        i++;
    }
    if (pCelulaLine->right->line == -1){ //a celula esta mais a direita
        pCelula->right = pCelulaLine->right; //aponta para a cabeca
        pCelulaLine->right = pCelula; //ultima celula ou cabeca, senao tiver celulas, aponta para a celula
    }
    //linha encontrada e apontada

    //Agora vamos encontrar a coluna
    //Percorre ate a coluna do elemento, percorre nas cabecas ate a informada
    for (i = 0; i < columns-1; i++){ //-1 pq ja ta na primeira
        pCelulaColumns = pCelulaColumns->right; //percorre a direita (colunas) ate achar a correta
    }
    i=0;
    while (i<line && pCelulaColumns->below->columns != -1){ //anda pelas celulas ate achar a posicao
        if (pCelulaColumns->below->line > pCelula->line){
            pCelula->below = pCelulaColumns->below;
            pCelulaColumns->below = pCelula;
        }
        else{
            pCelulaColumns = pCelulaColumns->below; //anda pelas celulas abaixo
        }
        i++;
    }
    if (pCelulaColumns->below->columns == -1){
        pCelula->below = pCelulaColumns->below; //aponta para a cabeca
        pCelulaColumns->below = pCelula; //ultima celula ou cabeca, senao tiver celulas, aponta para a celula
    }
    return 1;
}


int matrix_print(Matrix *mat){
    int i, j;
    tipo_celula *pCelula;

    if (!mat || !mat->m || !mat->n){
        return 0;
    }

    pCelula = mat->inicio->below;

    printf("\n");

    for (i = 1; i <= mat->m; i++){
        for (j = 1; j <= mat->n; j++){
            if (pCelula->right->line == i && pCelula->right->columns == j){
                pCelula = pCelula->right;
                printf("  \t%0.2f   ", pCelula->valor);
            }
            else{
                printf("  \t%0.2f   ", 0.0F);
            }
        }
        printf("\n");
        pCelula = pCelula->right->below;
    }

    return 1;
}

int matrix_destroy(Matrix *mat){
    int i = 1;
    int j = 1;
    tipo_celula *pCelula, *aux;

    if (!mat || !mat->m || !mat->n){
        return 0;
    }

    pCelula = mat->inicio->below;
    pCelula = pCelula->right;

    for (i = 1; i <= mat->m; i++){
        for (j = 1; j <= mat->n; j++){
            if (pCelula->line == i && pCelula->columns == j){
                aux = pCelula;
                pCelula = pCelula->right;
                free(aux);
            }
        }
        pCelula = pCelula->below->right;
    }

    //apaga cabecas
    pCelula = mat->inicio->right;

    for (i = 0; i < mat->n; i++){ //cabecas coluna
        aux = pCelula;
        pCelula = pCelula->right;
        free(aux);
    }

    pCelula = mat->inicio->below;
    for (i = 0; i < mat->m; i++){ //cabecas linha
        aux = pCelula;
        pCelula = pCelula->below;
        free(aux);
    }

    pCelula = mat->inicio;
    free(pCelula);

    mat->fimColumns = mat->fimLine = mat->inicio = NULL;
    mat->m = mat->n = 0;
    mat = NULL;

    return 1;
}

Matrix *matrix_add(Matrix *matA, Matrix *matB){
    int i, j;
    float soma;
    Matrix *matC;
    tipo_celula *pCelulaA, *pCelulaB;

    if (matA->m != matB->m || matA->n != matB->n){ //soma de matrizes so � possivel qndo as duas forem de mesma ordem
        printf("\nMatrizes de Ordem diferente.\nSo eh possivel somar matrizes de mesma ordem\n");
        return NULL;
    }

    if (!matA || !matB || !matA->m || !matA->n){
        return NULL;
    }

    matC = matrix_create(matA->m, matB->n);

    pCelulaA = matA->inicio->below;
    pCelulaB = matB->inicio->below;

    for (i = 1; i <= matA->m; i++){
        for (j = 1; j <= matA->n; j++){
            if (j == pCelulaA->right->columns && j == pCelulaB->right->columns){
                soma = pCelulaA->right->valor + pCelulaB->right->valor;
                if (soma){
                    matrix_setelem(matC, i, j, soma);
                }
                pCelulaA = pCelulaA->right;
                pCelulaB = pCelulaB->right;
            }
            else if (j == pCelulaA->right->columns){
                matrix_setelem(matC, i, j, pCelulaA->right->valor);
                pCelulaA = pCelulaA->right;
            }
            else if (j == pCelulaB->right->columns){
                matrix_setelem(matC, i, j, pCelulaB->right->valor);
                pCelulaB = pCelulaB->right;
            }
        }
        pCelulaA = pCelulaA->right->below;
        pCelulaB = pCelulaB->right->below;
    }

    return matC;
}

Matrix *matrix_multiply(Matrix *matA, Matrix *matB){
    int i=0, j=0, k=0;
    float total;
    Matrix *matC;

    if (matA->n != matB->m){ //so pode multiplicar se o numero de colunas de A eh igual ao numero de linhas de B
        printf("\nNao foi possivel multiplicar as matrizes\nnumero de colunas de A eh diferente do numero de linhas de B\n");
        return NULL;
    }

    if (!matA || !matB || !matA->m || !matA->n || !matB->n){
        return NULL;
    }

    matC = matrix_create(matA->m, matB->n); //C � formada pelo numero de linhas de A e de colunas de B

    for (i = 1; i <= matA->m; i++){
        for (j = 1; j <= matB->n; j++){
            total = 0;
            for (k = 1; k <= matA->n; k++){
                total += matrix_getelem(matA,i,k) * matrix_getelem(matB,k,j);
            }
            if (total){
                matrix_setelem(matC,i,j,total);
            }
        }
    }
    return matC;
}

float matrix_getelem(Matrix *mat, int line, int columns){
    tipo_celula *pCelula;
    int i = 0;

    pCelula = mat->inicio->right;

    for (i = 0; i < columns-1; i++){ //chega na coluna desejada
        pCelula = pCelula->right;
    }

    do{
        pCelula = pCelula->below;
        if (pCelula->line == line){
            return pCelula->valor;
        }
    }while(pCelula->columns != -1);

    return 0; //elemento nao encontrado
}