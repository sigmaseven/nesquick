#pragma once
#include <stdint.h>

enum memory_type
{
	MEMORY_TYPE_ROM,
	MEMORY_TYPE_RAM,
};

struct memory
{
	uint16_t size;
	uint8_t *data;
	enum memory_type type;
};

typedef enum memory_type memory_type_t;
typedef struct memory memory_t;

memory_t *nesquick_memory_init(memory_type_t m, uint16_t size);
void nesquick_memory_free(memory_t *memory);
