#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#define MAX_FILENAME 50

/*

MENU:

    USE: solicita nombre de la tabla y llama a createTable

    INSERT: solicita: clave y título a alamacenar
            llama: addTableEntry (error si no ha se utilizado antes USE)

    PRINT: imprime árbol binario por pantalla  (error si no se ha utilizado antes USE)

    EXIT: cierra el fichero, el índice y sale del programa
*/

char* use(void);
void insert(void);
void print(const char* fileName);

int main(void){
    int input;
    char *fileName = NULL;

    printf("Welcome!\n");
    do {
        printf("  (1) Use\n");
        printf("  (2) Insert\n");
        printf("  (3) Print\n");
        printf("  (4) Exit\n\n");
        printf("\nEnter a number that corresponds to your choice > ");
        scanf("%d", &input);

        switch (input) {
            case 1:
                fileName = use();
                break;
            case 2:
                if (fileName == NULL) {
                    fprintf(stderr, "Error. Ejecuta use primero\n");
                }
                else {
                    /*insert();*/
                }
                break;
            case 3:
                if (fileName == NULL) {
                    fprintf(stderr, "Error. Ejecuta use primero\n");
                }
                else {
                    print(fileName);
                }
                break;
            default:
                break;
        }
    } while (input != 4);

    if (fileName != NULL) {
        free(fileName);
    }

    return EXIT_SUCCESS;
}

char* use(void) {
    char *tableName;
    bool st = true;

    tableName = (char*) malloc(sizeof(tableName[0]) * MAX_FILENAME);
    if (tableName == NULL) {
        fprintf(stderr, "ERROR: fallo al reservar memoria en la funcion use");
        return NULL;
    }

    printf("Introduce el nombre del fichero (incluyendo .dat): ");
    fgets(tableName, MAX_FILENAME, stdin);
    fgets(tableName, MAX_FILENAME, stdin);
    tableName[strlen(tableName)-1] = '\0';


    st = createTable(tableName);
    if (st == false){
        fprintf(stderr, "Error al crear la tabla de datos\n");
        return NULL;
    }

    return tableName;
}

void insert(void) {
    char clave[PK_SIZE];
    char dataName[FILENAME_MAX], indexName[FILENAME_MAX];
    Book *book;
    bool st;

    printf("Introduce la clave a almacenar: ");
    fgets(clave, 4, stdin);

    printf("Introduce el título que desea almacenar: ");
    fgets(dataName, FILENAME_MAX, stdin);

    book = (Book *)malloc(sizeof(Book));
    if(book == NULL) {
        return;
    }
    book->title = (char *)malloc(sizeof(char) * (strlen(dataName) + 1));
    if(book->title == NULL) {
        free(book);
        return;
    }

    book->title_len = strlen(dataName);
    strcpy(book->book_id, clave);
    strcpy (book->title, dataName);

    st = addTableEntry(book, dataName, indexName);
    if (!st){
        fprintf(stderr, "Error introduciendo la clave en el fichero\n");
        return;
    }
}

void print(const char* fileName) {
    int level;
    char* indexName = NULL;

    indexName = (char*)malloc(sizeof(indexName[0]) * (strlen(fileName)+1));
    if (indexName == NULL) {
        fprintf(stderr, "Error: fallo al reservar memoria en funcion print\n");
        return;
    }

    replaceExtensionByIdx(fileName, indexName);

    createIndex (indexName);

    do {
        printf("\nHasta que profundidad quieres imprimir el arbol?\nIntroduce el valor >> ");
        scanf("%d", &level);
        if (level < 0) {
            fprintf(stderr, "Error: introduzca un valor >= 0\n");
        }
    } while(level < 0);

    printTree(level, indexName);

    free(indexName);
    return;
}