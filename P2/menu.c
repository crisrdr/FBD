#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <assert.h>
#include "customers.h"
#include "orders.h"
#include "products.h"
#include "odbc.h"

/******************************/
/* MENU PRODUCTS*/
    /* SUBMENU STOCK */
    /* SUBMENU FIND */
    /* BACK */
    

/* MENU ORDERS */
    /* SUBMENU OPEN */
    /* SUBMENU RANGE */
    /* SUBMENU DETAIL */
    /* BACK */

/* MENU CUSTOMERS */
    /* SUBMENU FIND */
    /* SUBMENU LIST PRODUCTS */
    /* SUBMENU BALANCE */
    /* BACK */

/* EXIT */
/****************************/

int main(void) {
    int input;
    int ret;
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;

    /* Inicio conexión */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }
  
    printf("¡Welcome!\n");
    /* Menu de Inicio*/    
    do {
        printf("  (1) Products\n");
        printf("  (2) Orders\n");
        printf("  (3) Customers\n");
        printf("  (4) Exit\n\n");
        printf("Enter a number that corresponds to your choice > ");
        (void) scanf("%d", &input);

        switch (input)
        {
        case 1:
            ProductsMenu(dbc);
            break;
        case 2:
            OrdersMenu(dbc);
            break;
        case 3:
            CustomersMenu(dbc);
            break;
        default:
            break;
        }
    } while (input != 4);

    /*Fin conexión*/
    printf("\n\nCLOSED CONNECTIONS\n");
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}