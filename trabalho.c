#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define elementos 10000000

int numberOfPartitions = 0;
int freezedElements = 0;
int numberOfElements = 0;

typedef struct Cliente
{
    int CodCliente;
    char Nome[50];
    char DataNascimento[8];

} Cliente;

typedef struct elemento
{
    Cliente cliente;
    short int congelado;
} Elemento;


void salvaCliente(FILE * arquivo, Cliente * aux){
    fwrite(&aux->CodCliente, sizeof(int), 1, arquivo);
    fwrite(aux->Nome, sizeof(aux->Nome), 1, arquivo);
    fwrite(aux->DataNascimento, sizeof(aux->DataNascimento), 1, arquivo);
}

int randomNumber(int max, int min){
    int aux = 0;
    aux = (rand() % (max - min + 1)) + min;
    return aux;
}

void trocaElementos(int * vet, int a, int b){
    int aux = vet[a];
    vet[a] = vet[b];
    vet[b] = aux;
}

void embaralhaVetor(int * vet, int iter, int tam){
    printf("========Embaralhando vetor========\n");
    int a, b;
    int per = 0;
    for(int i = 0; i < iter; i++){
        per = (int)((float) iter * 0.10); 
        if( i % per == 0)
            printf("Ja foi: %.0f %%\n", (float)i / (float)iter * 100.0);
        a = randomNumber(tam-1,0);
        b = randomNumber(tam-1,0);
        trocaElementos(vet, a, b);
    }
}

Cliente* criaCliente(int Cod, char * nome, char * data){

    Cliente * aux = malloc(sizeof(Cliente));
    aux->CodCliente = Cod;
    strcpy(aux->Nome, nome);
    strcpy(aux->DataNascimento, data);
    return aux;

}


Cliente * lerCliente(int cod, FILE * arquivo){
    Cliente * aux = malloc(sizeof(Cliente));
    rewind(arquivo);

    while (aux->CodCliente != cod)
    {
        if(0 >= fread(&aux->CodCliente, sizeof(int), 1, arquivo)){
            free(aux);
            break;
        }
        if(aux->CodCliente == cod){
            fread(&aux->Nome, sizeof(char), 50 , arquivo);
            fread(&aux->DataNascimento, sizeof(char), 8 , arquivo);
            return aux;
        }
        fseek(arquivo, sizeof(char) * 50, SEEK_CUR);
        fseek(arquivo, sizeof(char) * 8, SEEK_CUR);
    }
    
}

void imprimeCliente(Cliente * cliente){
    if(cliente == NULL){
        printf("Cliente inexistente!\n");
        return;
    }
    printf("\n===========Cliente=========\n\n");
    printf("Codigo: %i\n", cliente->CodCliente);
    printf("Nome: %s\n", cliente->Nome);
    printf("Data: %s\n\n", cliente->DataNascimento);
    printf("=============================\n\n");
}


void cadastraCliente(int cod, char * nome, char * data, FILE * arquivo){

    int aux;
    while (0 < fread(&aux, sizeof(int), 1, arquivo))
    {
        fseek(arquivo, sizeof(char) * 50, SEEK_CUR);
        fseek(arquivo, sizeof(char) * 8, SEEK_CUR);
    }
    
    Cliente * cliente = criaCliente(cod, nome, data);
    salvaCliente(arquivo, cliente);
    free(cliente);

}

void escreveCliente(FILE * arquivo, int cod, char * nome, char * data){
    Cliente * aux = criaCliente(cod, nome, data);
    salvaCliente(arquivo, aux);
}

Cliente * leCliente(FILE * arquivo){

    Cliente * aux = malloc(sizeof(Cliente));
    if (0 >= fread(&aux->CodCliente, sizeof(int), 1, arquivo))
    {
        return NULL;
    }
    fread(&aux->Nome, sizeof(char), 50 , arquivo);
    fread(&aux->DataNascimento, sizeof(char), 8 , arquivo);
    return aux;
}

void escreveNoDisco(FILE * arquivo, int qntd){
    int i = 0;
    int per = 0;
    int * vet;
    vet = malloc(sizeof(int) * qntd);
    for(i = 0; i < qntd; i++){
        vet[i] = i;
    }
    embaralhaVetor(vet, 50000000, qntd);
    printf("========Escrevendo no disco========\n");
    
    for (i = 0; i < qntd; i++)
    {   
        per = (int)((float) qntd * 0.10); 
        if( i % per == 0)
            printf("Ja foi: %.0f %%\n", (float)i / (float)qntd * 100.0);
        escreveCliente(arquivo, vet[i], "Gustavo Santos", "12062000");
    }
    free(vet);
    
}

FILE * createNewPartition(){
    numberOfPartitions++;
    char path[70];
    sprintf(path, "Partitions/Partition_%d.dat", numberOfPartitions);
    return fopen(path, "ab+");
}

int indexMenorElemento(Elemento * array, int m){
    int menor = 0;
    int i;
    for(i = 0; i < m; i++){
        if(array[i].congelado == 0){
            menor = i;
            break;
        }
    }
    for(i = 1; i < m; i++)
    {
        if(array[i].cliente.CodCliente < array[menor].cliente.CodCliente){
            if(array[i].congelado == 0){
                menor = i;
            }
        }
    }
    return menor;
}

void substituiNoArray(Elemento * array, int i, int m, FILE * dest, FILE * src){
    
    salvaCliente(dest, &array[i].cliente);
    Cliente * aux = leCliente(src);

    if(aux == NULL){
        numberOfElements--;
        free(aux);
        return;
    }

    if(aux->CodCliente > array[i].cliente.CodCliente){
        array[i].cliente = *aux;
        array[i].congelado = 1;
        freezedElements++;
    }
    else{
        array[i].cliente = *aux;
        array[i].congelado = 0;
    }
    free(aux);
}

void descongela(Elemento * array, int m){
    for(int i = 0; i < m; i++){
        array[i].congelado = 0;
    }
    freezedElements = 0;
}

void classificacaoExterna(FILE * arquivo, int m){
    int i = 0;
    Cliente * aux;
    Elemento * array = malloc(sizeof(Elemento) * m);
    numberOfElements = m;
    FILE * dest = createNewPartition();

    //Inicialização do array;
    for(i = 0; i < m; i++){
        aux = leCliente(arquivo);
        array[i].cliente = *aux;
        array[i].congelado = 0;
    }
    while (numberOfElements > 0)
    {
        if(freezedElements == m){
            fclose(dest);
            dest = createNewPartition();
            descongela(array, m);
        }
        substituiNoArray(array, indexMenorElemento(array,m), m, dest, arquivo);
    }
    
}

int main () {

    //FILE * arquivo = fopen("Partitions/Partition_2.dat", "ab+");
    FILE * arquivo = fopen("Clientes.dat", "ab+");
    classificacaoExterna(arquivo, 5000);
    fclose(arquivo);
    
    return 0;
}