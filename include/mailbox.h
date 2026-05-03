#include "stdint.h"
#ifndef MAILBOX_H
#define MAILBOX_H

#define TAG_ALLOCATE_BUFFER 0x00040001
#define TAG_RELEASE_BUFFER 0x00040002
#define TAG_GET_PHYS_WH 0x00040003
#define TAG_SET_PHYS_WH 0x00048003
#define TAG_GET_VIRT_WH 0x00040004
#define TAG_SET_VIRT_WH 0x00048004
#define TAG_GET_BITS_PER_PX 0x00040005
#define TAG_SET_BITS_PER_PX 0x00048005
#define TAG_GET_PIXEL_ORDER 0x00040006
#define TAG_SET_PIXEL_ORDER 0x00048006
#define TAG_GET_PITCH 0x00040008

typedef enum {
    POWER = 0,
    FB = 1,
    VUART = 2,
    VCHIQ =3,
    LEDS = 4,
    BTNS = 5,
    TOUCH = 6,
    COUNT = 7,
    PROP = 8
} mailbox_channels;

int mailbox_call(mailbox_channels channel , volatile uint32_t *buffer);

#endif