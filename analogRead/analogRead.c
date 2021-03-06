#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "timer.h"
#include "uart.h"
#include <stdio.h>

uint8_t string[40];

int16_t main(void) {
    init_clock();
    init_pin();
    init_timer();
    init_uart();

    printf("Analog Read code: \n");

    timer_setPeriod(&timer1, 0.05);
    timer_start(&timer1);

    int i;
    for (i = 0; i<6; i++) {
        pin_analogIn(&A[i]);
    }

    while (1) {
        printf(">> ");
        uart_gets(&uart1, string, 40);
        printf("You typed '%s'\n", string);
        // if (timer_flag(&timer1)) {
        //     timer_lower(&timer1);
        //     printf("%d %d\n", pin_read(&A[5]) >> 6);
        // }
    }
}
