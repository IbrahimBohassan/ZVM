#include "zvm_io.h"
#include<stdio.h>


bool zfnio_keyboard(zvm_vm_t *vm,  uint8_t port, uint8_t operation,  uint8_t argc){
    int character;
    scanf("%c", &character);
    printf("%c\n", character);
    vm->io_devices[port]->O[0] = character;

    return true;
}


bool zfnio_screen(zvm_vm_t *vm, uint8_t port, uint8_t operation,  uint8_t argc){
    
    /* functions:
      0: write one character
    */
    // 1, 0, argc
    uint8_t character = vm->io_devices[port]->I[0];
    printf("%c\n", character);
    return true;
}