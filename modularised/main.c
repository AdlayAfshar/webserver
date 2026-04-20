#include "server.h"

int main(void)
{
    start_server();
    return 0;
}

// cc main.c server.c routes.c state.c response.c products.c cart.c orders.c -o server