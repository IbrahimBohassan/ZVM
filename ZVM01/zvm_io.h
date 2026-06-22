#ifndef __ZVM_IO_H__
#define __ZVM_IO_H__
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "zvm.h"

#define ZVM_IO_DEVICE_STATUS_READY 0
#define ZVM_IO_DEVICE_STATUS_BUSY  1
#define ZVM_IO_DEVICE_STATUS_OFF   2

#define ZVM_IO_DEVICE_TYPE_IN    0
#define ZVM_IO_DEVICE_TYPE_OUT   1
#define ZVM_IO_DEVICE_TYPE_INOUT 2

#define ZVM_IO_PORT_KEYBOARD 0
#define ZVM_IO_PORT_SCREEN   1

#define ZVM_IO_SERVICE_SCREEN_WRITE_CHARACTER 0
#define ZVM_IO_SERVICE_KEYBOARD_READ_CHARACTER 0



typedef struct zvm_io_device_t zvm_io_device_t;
typedef bool (*device_processor_t)(zvm_vm_t *vm,  uint8_t port, uint8_t operation,  uint8_t argc);

struct zvm_io_device_t{
    uint8_t id;
    char *name;
    uint8_t status;
    uint8_t type;
    uint8_t I[4];
    uint8_t O[4];
    device_processor_t processor;
};


bool zfnio_keyboard(zvm_vm_t *vm,  uint8_t port, uint8_t operation,  uint8_t argc);
bool zfnio_screen(zvm_vm_t *vm, uint8_t port, uint8_t operation,  uint8_t argc);


static zvm_io_device_t keyboard = 
 {0, "keyboard", ZVM_IO_DEVICE_STATUS_READY, ZVM_IO_DEVICE_TYPE_IN, {0, 0, 0, 0}, {0, 0, 0, 0}, zfnio_keyboard};

static zvm_io_device_t screen = 
 {1, "screen", ZVM_IO_DEVICE_STATUS_READY, ZVM_IO_DEVICE_TYPE_OUT, {0, 0, 0, 0}, {0, 0, 0, 0}, zfnio_screen};

#endif