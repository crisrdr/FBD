#include <stdio.h>
#include "customers.h"
#include <sql.h>
#include <stdlib.h>
#include <string.h>
#include "odbc.h"

#define bufLenDefFind 72

/* Prototipos de las funciones privadas*/
static int ShowCustomersMenu();
static void CustomersFind(SQLHDBC dbc);
static void CustomersListP(SQLHDBC dbc);
static void CustomersBalance(SQLHDBC dbc);

/**
 * @brief implementa la funcionalidad del menú Customers
 * @author Cristina Rodríguez de los Ríos
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void CustomersMenu(SQLHDBC dbc){
    int nChoice = 0;
    do {
        nChoice = ShowCustomersMenu();
        switch (nChoice) {
            case 1:
                CustomersFind(dbc);
                break;
            case 2:
                CustomersListP(dbc);
                break;
            case 3:
                CustomersBalance(dbc);
                break;
            default:
                break;
        }
    } while (nChoice != 4);
}

/**
 * @brief imprime el menú de Customers por pantalla
 * @author Cristina Rodríguez de los Ríos
 * 
 * @return opción elegida por el usuario
 */
int ShowCustomersMenu(){
    int nSelected = 0;

    do{
        printf("\nCUSTOMERS MENU:\n\n");
        printf("(1) Find\n(2) List Products\n(3) Balance\n(4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        (void) scanf("%d", &nSelected);

        if ((nSelected < 1) || (nSelected > 4)){
            printf("\nYou entered an invalid choice. Try again.\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/**
 * @brief implementa el apartado Find del menú Customers
 * @author Cristina Rodríguez de los Ríos
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void CustomersFind(SQLHDBC dbc){
    char id[bufLenDefFind+1] ="%";
    int cnu = 0;
    char cna[bufLenDefFind], cfn[bufLenDefFind], cln[bufLenDefFind];
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2;
    int ret;
    /*Query*/
    char *txt = "select customernumber, customername, contactfirstname, contactlastname from customers where "
                "contactfirstname like ? or contactlastname like ? order by customernumber";


    printf("\nFIND QUERY\n\n");
    printf("Enter customer name > ");
    (void) fflush(stdout);
    (void) fgets(id + 1, bufLenDefFind, stdin);
    (void) fgets(id + 1, bufLenDefFind, stdin);
    id[strlen(id) -1] = '%';
    id[strlen(id)] = '\0';

    /* Allocate statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError allocating statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Prepare the statement */
    ret = SQLPrepare(stmt, (SQLCHAR*) txt, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError preparing statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Include the variables in the statement */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                           SQL_VARCHAR, (SQLULEN) (bufLenDefFind), 0, id, (SQLLEN) bufLenDefFind, NULL);

    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
                           SQL_VARCHAR, (SQLULEN) (bufLenDefFind), 0, id, (SQLLEN) bufLenDefFind, NULL);

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &cnu,  0, NULL);
    ret = SQLBindCol(stmt, 2, SQL_C_CHAR, &cna, bufLenDefFind, NULL);
    ret = SQLBindCol(stmt, 3, SQL_C_CHAR, &cfn, bufLenDefFind, NULL);
    ret = SQLBindCol(stmt, 4, SQL_C_CHAR, &cln, bufLenDefFind, NULL);
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%d %s %s %s\n", cnu, cna, cfn, cln);
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
 * @brief implemeenta el apartado List Products del menú Customers
 * @author Cristina Rodríguez de los Ríos Medina
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void CustomersListP(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2;
    int ret;
    char id[3];
    char pc[bufLenDefFind], pn[bufLenDefFind];
    int cu;
    /*Query*/
    char *txt = "select p.productcode, p.productname, sum(od.quantityordered) total_unidades from customers c natural "
                "join orders o natural join orderdetails od natural join products p where c.customernumber = ? group by "
                "p.productcode order by p.productcode";


    printf("\nLIST PRODUCTS QUERY\n\n");
    printf("Enter customer number > ");
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
                                SQL_C_CHAR, SQL_C_CHAR, 0, 0, &id, 3, NULL);

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError executing the statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_CHAR, &pc, bufLenDefFind, NULL); 
    ret = SQLBindCol(stmt, 2, SQL_C_CHAR, &pn, bufLenDefFind, NULL); 
    ret = SQLBindCol(stmt, 3, SQL_C_LONG, &cu, 0, NULL); 
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %s %d\n", pc, pn, cu);
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
 * @brief implementa el apartado Balance del menú Customers
 * @author Cristina Rodríguez de los Ríos
 * 
 * @param dbc parámetro para hacer la conexión con la base de datos
 */
void CustomersBalance(SQLHDBC dbc){
    char id[3];
    int ret;
    double balance = 0.0;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2;
    /*Query*/
    char *txt = "select (s1.pagos - s2.prod) balance from (select sum(p.amount) pagos from customers c natural join payments p where"
                    " c.customernumber = ?) as s1, (select sum(od.quantityordered * od.priceeach) prod from customers c natural join"
                    " orders o natural join orderdetails od where c.customernumber = ?) as s2";


    printf("\nBALANCE QUERY\n");
    printf("Enter customer number > ");
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
        odbc_extract_error("\nError preparando el statement", stmt, SQL_HANDLE_ENV);
        return;
    }  

    /* Include the variables in the statement */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,               
                                SQL_C_CHAR, SQL_C_CHAR, (SQLULEN) 0, 0, &id, 3, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,               
                                SQL_C_CHAR, SQL_C_CHAR, (SQLULEN) 0, 0, &id, 3, NULL);

    /* Execute statement */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("\nError ejecutando statement", stmt, SQL_HANDLE_ENV);
        return;
    }

    /* Extract the exit of the query and print results */
    ret = SQLBindCol(stmt, 1, SQL_C_DOUBLE, &balance, 0, NULL); 
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Balance = %.2lf\n", balance);
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