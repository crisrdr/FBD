#include <stdio.h>
#include <sql.h>
#include <stdlib.h>
#include <string.h>
#include "orders.h"
#include "odbc.h"

/* Prototipos de las funciones privadas */
static int ShowOrdersMenu();
static void OrdersOpen(SQLHDBC dbc);
static void OrdersRange(SQLHDBC dbc);
static void OrdersDetail(SQLHDBC dbc);
static void OrdersRange(SQLHDBC dbc);
static void OrdersDetail(SQLHDBC dbc);

/**
 * @brief implementa la funcionalidad del menú Orders
 * @author Miguel Lozano
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void OrdersMenu(SQLHDBC dbc){
    int nChoice = 0;
    do {
        nChoice = ShowOrdersMenu();
        switch (nChoice)
        {
        case 1:
            OrdersOpen(dbc);
            break;
        case 2:
            OrdersRange(dbc);
            break;
        case 3:
            OrdersDetail(dbc);
            break;
        default:
            break;
        }
    } while(nChoice!=4);

    return;
}

/**
 * @brief imprime el menú de Orders por pantalla
 * @author Miguel Lozano
 * 
 * @return opción elegida por el usuario
 */
int ShowOrdersMenu() {
    int nSelected = 0;

    do{
        printf("\nORDERS MENU:\n\n");
        printf("(1) Open\n(2) Range\n(3) Detail\n(4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        (void) scanf("%d", &nSelected);

        if ((nSelected < 1) || (nSelected > 4)){
            printf("\nYou entered an invalid choice. Try again.\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/**
 * @brief implementa el apartado Open del menú Orders
 * @author Miguel Lozano
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void OrdersOpen(SQLHDBC dbc) {
    SQLRETURN ret2;
    SQLHSTMT stmt = NULL;
    SQLINTEGER y = 0;
    int ret;
    /*Query*/
    char *txt = "select ordernumber from orders where shippeddate is NULL order by ordernumber";

    printf("\nNOT SHIPPED ORDERS QUERY\n\n");

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

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }


    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &y, 0, NULL);   /*Se está seleccionando solo 1 var*/
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%d\n", y);
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
 * @brief implementa el apartado Range del menú Orders
 * @author Miguel Lozano
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void OrdersRange(SQLHDBC dbc) {
    SQLRETURN ret2;
    SQLHSTMT stmt = NULL;
    SQLINTEGER orderNumber = 0;
    char orderDate[11];
    char shippedDate[11];
    char dateInit[20], dateEnd[20], input[40];
    char *token;
    int ret;
    /*Query*/
    char *txt = "select ordernumber, orderdate, shippeddate from orders where orderdate >= ? and orderdate <= ? order by ordernumber";

    /* Allocate statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    printf("\nCONSULTA DE PEDIDOS ENTRE DOS FECHAS\n\n");
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void) fgets(input, 30, stdin);
    (void) fgets(input, 30, stdin);
    token = strtok(input, " ");
    strcpy(dateInit, token);
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    strcpy(dateEnd, token);

    /* Prepare the statement */
    ret = SQLPrepare(stmt, (SQLCHAR*) txt, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }    

    /* Include the variables in the statement */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,                 /* Por que sustituimos la ? || 1: primer argumento posicional*/
                                SQL_C_CHAR, SQL_C_CHAR, 0, 0, &dateInit, 11, NULL);         /*Se asigna al valor de la x*/

    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,                 /* Por que sustituimos la ? || 2: primer argumento posicional*/
                                SQL_C_CHAR, SQL_C_CHAR, 0, 0, &dateEnd, 11, NULL);         /*Se asigna al valor de la x*/

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &orderNumber, 0, NULL);
    ret = SQLBindCol(stmt, 2, SQL_C_CHAR, &orderDate, 20, NULL);
    ret = SQLBindCol(stmt, 3, SQL_C_CHAR, &shippedDate, 20, NULL);
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%d %s %s\n", orderNumber, orderDate, shippedDate);;
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
 * @brief implementa el apartado Detail del menú Orders
 * @author Miguel Lozano
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void OrdersDetail(SQLHDBC dbc) {
    SQLRETURN ret2;
    SQLHSTMT stmt = NULL;
    SQLINTEGER orderNumber = 0;
    char orderDate[11];
    char status[11];
    char productCode[15];
    SQLINTEGER quatityOrdered;
    SQLDOUBLE totalCost;
    SQLDOUBLE priceEach;
    int ret;
    /*Queries*/
    char *query1 = "select o.orderdate, o.status, sum(o2.priceeach) from orders o natural join orderdetails o2 where o.ordernumber = ? group by o.ordernumber";
    char *query2 = "select o2.productcode, o2.quantityordered, o2.priceeach from orders o natural join orderdetails o2 where o.ordernumber = ? order by o2.orderlinenumber";

    /* Allocate statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    printf("\nCONSULTA DETALLES DE UN PEDIDO\n\n");
    printf("Enter ordernumber > ");
    (void) fflush(stdout);
    (void) scanf("%d", &orderNumber);

    /* PRIMERA QUERY */
    ret = SQLPrepare(stmt, (SQLCHAR*) query1, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }    

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,                 
                                SQL_C_SHORT, SQL_C_SHORT, 0, 0, &orderNumber, 0, NULL);

    (void) SQLExecute(stmt);

    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, &orderDate, 11, NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, &status, 11, NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_DOUBLE, &totalCost, 0, NULL);

    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("\nOrderdate = %s    Status = %s\nTotal Cost = %.2lf\n", orderDate, status, totalCost);
    }

    (void) SQLCloseCursor(stmt);

    /* Free of the statement used */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("\nError liberando el statement", stmt, SQL_HANDLE_STMT);
        return;
    }

    /* SEGUNDA QUERY */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    ret = SQLPrepare(stmt, (SQLCHAR*) query2, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparando el statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,                 
                                SQL_C_SHORT, SQL_C_SHORT, 0, 0, &orderNumber, 0, NULL);

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }
    
    /* Extract the exit of the query and print results */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, &productCode, 15, NULL);
    (void) SQLBindCol(stmt, 2, SQL_INTEGER, &quatityOrdered, 0, NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_DOUBLE, &priceEach, 0, NULL);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("\n%s %d %.2lf", productCode, quatityOrdered, priceEach);
    }

    (void) SQLCloseCursor(stmt);   

    /* Free of the statement used */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("\nError freeing the statement", stmt, SQL_HANDLE_STMT);
        return;
    }

    return;
}