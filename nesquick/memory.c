#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

memory_t* nesquick_memory_init(memory_type_t m, uint16_t size) 
{
	memory_t* memory = (memory_t *)malloc(sizeof(memory_t));
	memory->size = size;
	memory->data = (uint8_t *)malloc(size);
	memset(memory->data, 0, size);
	memory->type = m;
	return memory;
}

void nesquick_memory_free(memory_t *memory)
{
	free(memory->data);
	free(memory);
}

void nesquick_memory_load(memory_t *memory, char *location)
{
	FILE *f = fopen(location, "rb");

	if(f == NULL)
	{
		// error
		return;
	}
	
	if(fread(memory->data, memory->size, 1, f) == 0)
	{
		//error
		fclose(f);
		return;
	}

	fclose(f);
}

void nesquick_memory_load_rom(memory_t *memory, char *location)
{
	
}