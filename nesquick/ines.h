#pragma once
#include <stdint.h>

#include "memory.h"

struct ines_header
{
	uint32_t magic;
	uint8_t prg_size;
	uint8_t chr_size;
	uint8_t flags1;
	uint8_t flags2;
	uint8_t flags3;
	uint8_t flags4;
	uint8_t flags5;
	uint8_t padding[5];
};

typedef struct ines_header ines_header_t;