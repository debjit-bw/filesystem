/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define CONTENT_OFFSET (512 * 1024)
#define FLAG_OFFSET (513 * 1024)
#define FLAG_INT 5

const uint8_t *data_array = (const uint8_t *) (XIP_BASE + CONTENT_OFFSET);
uint8_t data_array_working[FLASH_PAGE_SIZE] = {0};
// const uint8_t *flag_content = (const uint8_t *) (XIP_BASE + FLAG_OFFSET);

// void print_buf(const uint8_t *buf, size_t len) {
//     for (size_t i = 0; i < len; ++i) {
//         printf("%d", buf[i]);
//         if (i % 16 == 15)
//             printf("\n");
//         else
//             printf(" ");
//     }
// }

// void draw_number(int x)
// {
    
//     free(str);
// }

int main() {
    stdio_init_all();
    stdio_usb_init();
    // sleep_ms(2000);

    uint32_t ints = save_and_disable_interrupts();
    restore_interrupts (ints);

    uint8_t flag = data_array[0];
    printf("Flag is: %d.\n", flag);
    uint8_t number = data_array[1];
    printf("Number is: %d.\n", number);

    if (flag == FLAG_INT)
    {
        data_array_working[0] = (uint8_t) FLAG_INT;
        printf("Flag value appears set at %d. entering loop.\n", data_array_working[0]);
        char str[10] = {0};

        while(true)
        {
            number = data_array[1];
            printf("The current number is: %d\n", number);
            data_array_working[1] = (number + (uint8_t)1) % (uint8_t)200;
            // printf("The current number is: %d\n", data_array_working[1]);
            ints = save_and_disable_interrupts();
            flash_range_erase(CONTENT_OFFSET, FLASH_SECTOR_SIZE);
            flash_range_program(CONTENT_OFFSET, data_array_working, FLASH_PAGE_SIZE);
            restore_interrupts(ints);

            int length = snprintf(NULL, 0, "%d", number);
            printf("Size: %d.\n", length);
            snprintf(str, length + 1, "%d", number);

            sleep_ms(200);
        }
    }
    else
    {
        printf("Clearing...\n\n");
        // uint8_t data_array[FLASH_PAGE_SIZE];
        // for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
        //     data_array[i] = 0;
        
        data_array_working[0] = FLAG_INT;
        data_array_working[1] = 0;
        printf("Flag value not set. Setting flag at %d and number at %d.\n", data_array_working[0], data_array_working[1]);
        ints = save_and_disable_interrupts();
        flash_range_erase(CONTENT_OFFSET, FLASH_SECTOR_SIZE);
        flash_range_program(CONTENT_OFFSET, data_array_working, FLASH_PAGE_SIZE);
        // flash_range_program(FLAG_OFFSET, &flag_u, sizeof(int));
        // flash_range_erase(NUMBER_OFFSET, sizeof(int));
        // flash_range_program(NUMBER_OFFSET, &number_u, sizeof(int));
        restore_interrupts(ints);
        printf("Successfully set number at %d and flag at %d.\n", data_array_working[1], data_array_working[0]);
    }

    // printf("Generated random data:\n");
    // print_buf(random_data, FLASH_PAGE_SIZE);

    // // Note that a whole number of sectors must be erased at a time.
    // printf("\nErasing target region...\n");
    // uint32_t ints = save_and_disable_interrupts();
    // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    // restore_interrupts (ints);
    // printf("Done. Read back target region:\n");
    // print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    // printf("\nProgramming target region...\n");
    // ints = save_and_disable_interrupts();
    // flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);
    // restore_interrupts (ints);
    // printf("Done. Read back target region:\n");
    // print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    // bool mismatch = false;
    // for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
    //     if (random_data[i] != flash_target_contents[i])
    //         mismatch = true;
    // }
    // if (mismatch)
    //     printf("Programming failed!\n");
    // else
    //     printf("Programming successful!\n");
}
