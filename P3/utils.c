#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

#define BYTE_TO_BITE 8

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    const char *extension = "idx";

    strcpy(indexName, fileName);
    indexName[strlen(indexName)-3] = '\0';
    strcat(indexName, extension);

    return;
}


bool createTable(const char * tableName) {
    FILE *fp;
    int buf = -1;
    bool st = true;
    char *indexName = NULL;

    indexName = (char *)malloc(sizeof(char) * (strlen(tableName) + 1));
    if(indexName == NULL) {
        return false;
    }

    fp = fopen(tableName, "r");
    if(fp == NULL) {
        fp = fopen(tableName, "wb+");
        fwrite(&buf, sizeof(buf), 1, fp);
        fclose(fp);

        return true;
    }

    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0) {
        fclose(fp);
        fp = fopen(tableName, "wb+");
        fwrite(&buf, sizeof(buf), 1, fp);
        fclose(fp);
        replaceExtensionByIdx(tableName, indexName);
        st = createIndex(indexName);
        if (st == false){
            fprintf(stderr, "Error al crear la tabla de índices\n");
            return NULL;
        }

        return true;
    }

    fclose(fp);
    return true;
}

bool createIndex(const char *indexName) {
    FILE *fp;
    int proot = -1;
    int pdelete = -1;

    fp = fopen(indexName, "r");
    if(fp == NULL) {
        fp = fopen(indexName, "wb+");
        fwrite(&proot, sizeof(proot), 1, fp);
        fwrite(&pdelete, sizeof(pdelete), 1, fp);
        fclose(fp);
        return true;
    }

    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0) {
        fclose(fp);
        fp = fopen(indexName, "wb+");
        fwrite(&proot, sizeof(proot), 1, fp);
        fwrite(&pdelete, sizeof(pdelete), 1, fp);
        fclose(fp);
        return true;
    }

    fclose(fp);
    return true;
}


void printnode(size_t _level, size_t level, FILE * fp, int node_id, char symbol) {
    char pk[PK_SIZE];
    int left, right, offset, i;


    /* Caso base de la recursión */
    if(_level > level || node_id == -1) return;
    
    /* Nos posicionamos en el reistro objetivo */
    fseek(fp, node_id*INDEX_REG_SIZE+INDEX_HEADER_SIZE, SEEK_SET);

    /* Leemos los datos del Node */
    fread(pk, sizeof(char), 4, fp);
    fread(&left, sizeof(int), 1, fp);
    fread(&right, sizeof(int), 1, fp);
    fseek(fp, sizeof(int), SEEK_CUR);
    fread(&offset, sizeof(int), 1, fp);

    for(i=0; i<(int)_level; i++) {
        printf("      ");
    }
    /* Imprimimos el nodo */
    printf("%c %s (%d): %d\n", symbol,pk, node_id, offset);
    /* Hijo izquierdo */
    printnode(_level+1, level, fp, left, 'l');
    /* Hijo derecho */
    printnode(_level+1, level, fp, right, 'r');

    return;
}


void printTree(size_t level, const char * indexName) {
    FILE *fp = NULL;
    int root;

    fp = fopen(indexName, "rb");
    if(fp == NULL) {
        fprintf(stderr, "ERROR: El archivo %s no existe\n", indexName);
        return;
    }

    /* Leemos de la cabecera el nodo del árbol */
    fread(&root, sizeof(int), 1, fp);
    if(root == -1) return;

    /* Imprimimos el árbol */
    printnode(0, level, fp, root, ' ');

    fclose(fp);
    return;
}

bool findKey(const char *book_id, const char *indexName, int * nodeIDOrDataOffset) {
    FILE *fp;
    char key[8];
    int cmp;
    int parent;

    fp = fopen(indexName,"rb+");

    /* Leemos cuál es el nodo raíz */
    fread(nodeIDOrDataOffset, sizeof(int), 1, fp);
    do {
        /* Nos situamos en la clave */
        fseek(fp, (*nodeIDOrDataOffset)*INDEX_REG_SIZE + INDEX_HEADER_SIZE, SEEK_SET);
        parent = *nodeIDOrDataOffset;
        /* Leemos la clave */
        fread(key, sizeof(char), 4, fp);

        cmp = strncmp(book_id, key, 4);
        if (cmp == 0) {
            fseek(fp, (*nodeIDOrDataOffset) * INDEX_REG_SIZE + INDEX_HEADER_SIZE + INDEX_REG_SIZE - 4, SEEK_SET);
            fread(nodeIDOrDataOffset, sizeof(int), 1, fp);
            fclose(fp);
            return true;
        }
        else if (cmp < 0) {
            fread(nodeIDOrDataOffset, sizeof(int), 1, fp); /* Vamos al hijo izquierdo */
        }
        else {
            fseek(fp, 4, SEEK_CUR); /* Vamos al hijo derecho */
            fread(nodeIDOrDataOffset, sizeof(int), 1, fp);
        }
    } while((*nodeIDOrDataOffset) != -1);

    *nodeIDOrDataOffset = parent;
    fclose(fp);
    return false;
}


bool addTableEntry(Book * book, const char * dataName, const char * indexName) {
    int nodeIDOrDataOffset;
    int borrado;
    bool st;
    FILE *fp;
    size_t n_size = sizeof(Node);
    size_t reg;
    Node node;
    
    /* Comprueba si la clave existe*/
    st = findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    if (st) {
        return false;
    }
    
    /* Comprueba si hay registros borrados*/
    fp = fopen(indexName, "rb+");
    if (fp == NULL) return false;

    fread(&borrado, sizeof(borrado), 1, fp);

    if (borrado == -1){ /* Si no hay registros borrados, añadir al final */
        reg = BYTE_TO_BITE + n_size * borrado;
        /* Posicionamiento en el último registro */
        /*fseek(fp, reg, SEEK_SET);*/
        fseek(fp, 0, SEEK_END); /*Miguel*/
        /*fread(&node, n_size, 1, fp);*/
        fwrite(book->book_id, sizeof(book->book_id[0]), 4, fp); /*Miguel*/
        
    } else { /* Si hay registros borrados comprobar el tamaño de estos*/

    }
        

    return true;
}

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    return true;
}