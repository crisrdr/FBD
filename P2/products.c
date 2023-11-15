#include <stdio.h>
#include <sql.h>
#include <stdlib.h>
#include <string.h>
#include "products.h"
#include "odbc.h"

#define bufLenDefFind 72

/* Prototipos de funciones privadas */
static int ShowProductsMenu();
static void ProductsStock(SQLHDBC dbc);
static void ProductsFind(SQLHDBC dbc);

/**
 * @brief implementa la funcionalidad del menú Products
 * @author Cristina Rodríguez de los Ríos
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void ProductsMenu(SQLHDBC dbc){
    int nChoice = 0;
    do {
        nChoice = ShowProductsMenu();
        switch (nChoice) {
            case 1:
                ProductsStock(dbc);
                break;
            case 2:
                ProductsFind(dbc);
                break;
            default:
                break;
        }
    } while (nChoice != 3);
}

/**
 * @brief imprime el menú de Products por pantalla
 * @author Cristina Rodríguez de los Ríos
 * 
 * @return opción elegida por el usuario
 */
int ShowProductsMenu(){
    int nSelected = 0;

    do{
        printf("\n\nPRODUCTS MENU:\n\n");
        printf("(1) Stock\n(2) Find\n(3) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        (void) scanf("%d", &nSelected);

        if ((nSelected < 1) || (nSelected > 3)){
            printf("\nYou entered an invalid choice. Try again.\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}

/**
 * @brief implementa el apartado Stock del menú Products
 * @author Cristina Rodríguez de los Ríos
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void ProductsStock(SQLHDBC dbc){
    SQLRETURN ret2;
    SQLHSTMT stmt = NULL;
    SQLINTEGER y = 0;
    char id[8];
    int ret;
    /*Query*/
    char *txt = "select quantityinstock from products where productcode = ?";


    printf("\nSTOCK QUERY\n\n");
    printf("Enter productcode > ");
    (void) fflush(stdout);
    (void) scanf("%s", id);

    /* Allocate statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError allocating statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Prepare the statement */
    ret = SQLPrepare(stmt, (SQLCHAR*) txt, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }    

    /* Include the variables in the statement */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, 
                                SQL_C_CHAR, SQL_C_CHAR, 0, 0, &id, 8, NULL);
    
    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &y, 0, NULL); 
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Number of products in stock: %d\n", y);
    }

    (void) SQLCloseCursor(stmt);   
    
    printf("\n");

    /* Free of the statement used */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("\nError freeing the statement", stmt, SQL_HANDLE_STMT);
        return;
    }
}

/**
 * @brief implementa el apartado Find del menú Products
 * @author Cristina Rodríguez de los Ríos y Miguel Lozano
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void ProductsFind(SQLHDBC dbc){
    char col1[bufLenDefFind];
    char col2[bufLenDefFind];
    col1[0] = '\0';
    col2[0] = '\0';
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2;
    int ret;
    char id[bufLenDefFind] ="%";
    /*Query*/
    char *txt = "select productcode, productname from products where productname like ? order by productcode";

    printf("\nCONSULTA DE FIND\n\n");
    printf("Enter productname > ");
    (void) fflush(stdout);
    (void) fgets(id + 1, bufLenDefFind, stdin);
    (void) fgets(id + 1, bufLenDefFind, stdin);
    id[strlen(id) -1] = '%';
    id[strlen(id)] = '\0';

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError allocating statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Prepare the statement */
    ret = SQLPrepare(stmt, (SQLCHAR*) txt, SQL_
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparing statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Include the variables in the statement */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                           SQL_VARCHAR, (SQLULEN) (bufLenDefFind), 0, id, (SQLLEN) bufLenDefFind, NULL);
 
    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError at the second BindParameter", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_CHAR, &col1,  bufLenDefFind, NULL);
    ret = SQLBindCol(stmt, 2, SQL_C_CHAR, &col2, bufLenDefFind, NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("\n%s %s", col1, col2);
    }

    (void) SQLCloseCursor(stmt);   
    
    printf("\n");

    /* Free of the statement used */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("\nError freeing the statement", stmt, SQL_HANDLE_STMT);
        return;
    }
}