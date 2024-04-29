/*
 * O Jogo da Vida
 *
 * Uma célula nasce se tiver exatamente três vizinhos.
 * Uma célula morre de solidão se tiver menos de dois vizinhos.
 * Uma célula morre de superpopulação se tiver mais de três vizinhos.
 * Uma célula sobrevive à próxima geração se não morrer de solidão
 * ou superpopulação.
 *
 * Nesta versão, é usado um array 2D de ints. 1 indica célula viva, 0
 * indica célula morta. O jogo avança por um número de passos (dado pela entrada),
 * imprimindo na tela a cada passo. 'x' impresso significa célula viva, espaço significa célula morta.
 */

//Inclusao de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//definicao de variaveis
#define MASTER 0

typedef unsigned char celula_t;

celula_t **alocar_tabuleiro(int tamanho) {
    celula_t **tabuleiro = (celula_t **) malloc(sizeof(celula_t*) * tamanho);
    int i;
    for (i = 0; i < tamanho; i++)
        tabuleiro[i] = (celula_t *) malloc(sizeof(celula_t) * tamanho);
    return tabuleiro;
}

void liberar_tabuleiro(celula_t **tabuleiro, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++)
        free(tabuleiro[i]);
    free(tabuleiro);
}

/* Retorna o número de células vivas adjacentes à célula (i, j) */
int celulas_adjacentes(celula_t **tabuleiro, int tamanho, int i, int j) {
    int k, l, count = 0;
   
    int sk = (i > 0) ? i - 1 : i;
    int ek = (i + 1 < tamanho) ? i + 1 : i;
    int sl = (j > 0) ? j - 1 : j;
    int el = (j + 1 < tamanho) ? j + 1 : j;

    for (k = sk; k <= ek; k++)
        for (l = sl; l <= el; l++)
            count += tabuleiro[k][l];
    count -= tabuleiro[i][j];
   
    return count;
}

void jogar(celula_t **tabuleiro, celula_t **novo_tabuleiro, int tamanho, int anterior) {
    int i, j, a;
    /* Para cada célula, aplicar as regras do Jogo da Vida */
    for (i=anterior; i < tamanho; i++){
        for (j = 0; j < tamanho; j++) {
            a = celulas_adjacentes(tabuleiro, tamanho, i, j);
            if (a == 2)
                novo_tabuleiro[i][j] = tabuleiro[i][j];
            if (a == 3)
                novo_tabuleiro[i][j] = 1;
            if (a < 2)
                novo_tabuleiro[i][j] = 0;
            if (a > 3)
                novo_tabuleiro[i][j] = 0;
        }
    }
}

/* Imprime o tabuleiro do Jogo da Vida */
void imprimir(celula_t **tabuleiro, int tamanho) {
    int i, j;
    /* Para cada linha */
    for (j = 0; j < tamanho; j++) {
        /* Imprimir cada posição da coluna... */
        for (i = 0; i < tamanho; i++)
            printf("%c", tabuleiro[i][j] ? 'x' : ' ');
        /* Seguido por uma quebra de linha */
        printf("\n");
    }
}

/* Lê um arquivo para inicializar o tabuleiro do Jogo da Vida */
void ler_arquivo(FILE *f, celula_t **tabuleiro, int tamanho) {
    int i, j;
    char *s = (char *) malloc(tamanho + 10);
    char c;
    for (j = 0; j < tamanho; j++) {
        /* Obter uma string */
        fgets(s, tamanho + 10, f);
        /* Copiar a string para o tabuleiro do Jogo da Vida */
        for (i = 0; i < tamanho; i++)
            tabuleiro[i][j] = s[i] == 'x';
    }
}
//funcao main
int main() {
	//definicao de variaveis
    int tamanho, passos, tag=1, numtasks, rank;
   
    FILE *f;
    
    MPI_Status Stat;
    MPI_Init(NULL, NULL); // Inicia o MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Obtém o número de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    
    //entrada e saida do arquivo por MASTER
    if(rank==MASTER){
    	f = stdin;
    	fscanf(f, "%d %d", &tamanho, &passos);
    	MPI_Bcast(&tamanho, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
      	MPI_Bcast(&passos, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    }
    else{
       	MPI_Bcast(&tamanho, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
       	MPI_Bcast(&passos, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    }
   
    //array que contem tamanho da fatia de cada processo
    int new_Tamanho[numtasks];
   
   //verificacao de resto 
    if(tamanho%numtasks==0){
        for(int i=0; i<numtasks; i++){
            new_Tamanho[i] = tamanho/numtasks;
        }
    }
    else{
        int resto;
        int j=0;
        for(int i=0; i<numtasks; i++){
            new_Tamanho[i] = tamanho/numtasks;
        }
        resto = tamanho%numtasks;
        while (resto>0){
            new_Tamanho[j]++;
            resto--;
        }    
    }
    
   //montagem da estrutura 2x2
    int inicial=0;
    int final = new_Tamanho[0]-1;
    int estrutura[4][numtasks];
    for (int i=0;i<4;i++){
    	for(int j=0; j<numtasks; j++){
    		if (i==0){
    			estrutura[i][j]= inicial;
    			inicial = inicial + new_Tamanho[j];
    		}
    		else if (i==1){
    			estrutura[i][j]= final;
    			final = final + new_Tamanho[j];
    		}
    		else if (i==2){
    			estrutura[i][j]= new_Tamanho[j];
    		}
    		else if (i==3){
    			if(j==0){
    				estrutura[i][j]= new_Tamanho[j]+1;
    			}
    			else if(j==numtasks-1){
    				estrutura[i][j]= new_Tamanho[j]+1;
    			}
    			else{
    				estrutura[i][j]= new_Tamanho[j]+2;
    			}
    		}	
    	}
    }
    
    //alocar no tabuleiro
    celula_t **anterior = alocar_tabuleiro(tamanho);
    if(rank==MASTER){
    ler_arquivo(f, anterior, tamanho);
    fclose(f);
    }
    celula_t **proximo = alocar_tabuleiro(tamanho);
    celula_t **tmp;
    int i, j;
    
    //envio das matrizes de cada processo
    if (rank == MASTER) {
        for (int i = 1; i < numtasks; i++) {
            MPI_Send(&(anterior[0][estrutura[0][i]]), estrutura[2][i] * tamanho, MPI_UNSIGNED_CHAR, i, tag, MPI_COMM_WORLD);
        }
    } 
    else {
        MPI_Recv(&(anterior[0][estrutura[0][rank]]), estrutura[2][rank] * tamanho, MPI_UNSIGNED_CHAR, MASTER, tag, MPI_COMM_WORLD, &Stat);
    }
	
	//Execução do jogo em cada processo
    for (i = 0; i < passos; i++) {
		jogar(anterior, proximo, tamanho, rank);

        tmp = proximo;
        proximo = anterior;
        anterior = tmp;
    }
    
    if(rank==MASTER){
    imprimir(anterior, tamanho);
    }
    liberar_tabuleiro(anterior, tamanho);
    liberar_tabuleiro(proximo, tamanho);

    MPI_Finalize(); // Finaliza o MPI

    return 0;
}

