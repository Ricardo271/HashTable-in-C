#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 151                  // 31 - 79 - 151
#define INPUT_FILE "DEZ_MIL"  // "CEM" - "MIL" - "DEZ_MIL"

#define MAX_STRING 256  // Máximo de caracteres na string

int COLLISON_COUNTER = 0;

typedef struct Data {
    char str[MAX_STRING];
    struct Data* next;
} Data;

Data* hash_table[M];

unsigned int hash_function(char* str) {
    int length = strlen(str);
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++) {
        hash_value += str[i];
    }
    return hash_value % M;
}

void print_table() {
    printf("INICIO\n");
    for (int i = 0; i < M; i++) {
        if (hash_table[i] == NULL) {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t", i);
            Data* aux = hash_table[i];
            while (aux != NULL) {
                printf("%s - ", aux->str);
                aux = aux->next;
            }
            printf("\n");
        }
    }
    printf("FIM\n");
}

/*  Inicia a Tabela Hash com todos os valores Nulos */
void init() {
    for (int i = 0; i < M; i++) {
        hash_table[i] = NULL;
    }
}

/*  Função para inserir um valor na Tabela Hash
    Recebe um Struct para ser inserido na Tabela
    Retorna True se a operação tiver sucesso e False se o valor recebido for inválido
*/
bool insert(Data data) {
    Data* new_data = malloc(sizeof(Data));
    if (new_data == NULL) {
        exit(2);
    }

    strcpy(new_data->str, data.str);

    int index = hash_function(new_data->str);
    if (hash_table[index] != NULL) {
        COLLISON_COUNTER++;
    }
    new_data->next = hash_table[index];
    hash_table[index] = new_data;
    return true;
}

/*  Função para buscar um valor na Tabela Hash
    Recebe uma String para ser buscada.
    Retorna um ponteiro para a Struct que contém aquele valor
*/
Data* search(char* str) {
    int index = hash_function(str);
    Data* aux = hash_table[index];
    while (aux != NULL && strcmp(aux->str, str) != 0) {
        aux = aux->next;
    }
    return aux;
}

/*  Função para remover um valor da Tabela Hash.
    Recebe uma String com o valor para ser buscado e removido.
    Retorna um ponteiro para o Struct na tabela que continha aquele valor
    para que a memória possa ser liberada.
 */
Data* delete (char* str) {
    int index = hash_function(str);
    Data* aux = hash_table[index];
    Data* prev = NULL;
    while (aux != NULL && strcmp(aux->str, str) != 0) {
        prev = aux;
        aux = aux->next;
    }

    if (aux == NULL) {
        // Se str não foi encontrado na lista
        return NULL;
    }

    if (prev == NULL) {
        // Se str for o primeiro valor da lista
        hash_table[index] = aux->next;
    } else {
        // Se str estiver em outro lugar
        prev->next = aux->next;
    }

    return aux;  // Retorna o ponteiro para que a memória possa ser liberada
}

void remove_line_break(char str[]) {
    int i = 0;
    while (str[i] != '\n' && str[i] != '\000') {
        i++;
    }
    str[i] = '\000';
}

int main() {
    init();  // Inicia a Tabela Hash

    // Seleciona o arquivo de entrada das Strings
    FILE* fp;
    if (INPUT_FILE == "CEM") {
        fp = fopen("./strings_cem.txt", "r");
    } else if (INPUT_FILE == "MIL") {
        fp = fopen("./strings_mil.txt", "r");
    } else if (INPUT_FILE == "DEZ_MIL") {
        fp = fopen("./strings_dez_mil.txt", "r");
    } else {
        printf("Erro na selecao do arquivo de entrada\n");
    }

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(2);
    }

    // Insere todas as Strings na Tabela Hash
    char str[MAX_STRING];
    while (fgets(str, MAX_STRING, fp) != NULL) {
        Data input;
        remove_line_break(str);
        strcpy(input.str, str);
        insert(input);
    }

    fclose(fp);

    print_table();

    char* searches[] = {"acodem", "acelerasses", "aconchegados", "acatavamos", "abalizaste", "aburguesaramos", "afluimo", "adormecias", "nao encontrar", "123abc"};
    int searches_c = sizeof(searches) / sizeof(char*);
    long time_before_searches = clock();
    for (int i = 0; i < searches_c; i++) {
        Data* aux = search(searches[i]);
        if (aux == NULL) {
            printf("Nao encontrado.\n");
        } else {
            printf("Encontrado: %s\n", aux->str);
        }
    }
    long time_after_searches = clock();
    long time_elapsed_searches = (time_after_searches - time_before_searches);

    printf("Time Before: %ld\n", time_before_searches);
    printf("Time After: %ld\n", time_after_searches);
    printf("Time Elapsed: %ld\n", time_elapsed_searches);

    printf("COLISOES DETECTADAS: %d\n", COLLISON_COUNTER);
}
