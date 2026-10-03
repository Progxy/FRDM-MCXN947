#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdint.h>
#include "./utils.h"

// TODO: Could define an allocator structure in order to define multiple
//       allocators on smaller chunks of memory (Matrioska of allocators).
//       But for that should revise the block_hdr_t structure (as it consumes a block).

extern uint32_t _ram_start;
extern uint32_t _ram_end;

// NOTE: before modifying this value should also modify _block_size in the linker.ld
#define BLOCK_SIZE 64

static const void* mem_base = (void*) (&_ram_start);
static const void* mem_end  = (void*) (&_ram_end);

typedef struct block_hdr_t {
	unsigned int block_size;
	struct block_hdr_t* next_block;
	struct block_hdr_t* prev_block;
	uint8_t is_free;
} __attribute__((aligned(BLOCK_SIZE))) block_hdr_t;

static_assert(sizeof(block_hdr_t) == BLOCK_SIZE, "Size must be BLOCK_SIZE bytes");

void insert_next_block(block_hdr_t* first_block, block_hdr_t* last_block) {
	block_hdr_t* next_block = first_block + first_block -> block_size + 1;
	if (last_block == NULL) last_block = first_block -> next_block;

	if (next_block >= (block_hdr_t*) mem_end) {
		first_block -> next_block = NULL;
	} else if (last_block == NULL) {
		next_block -> block_size = (unsigned int) ((block_hdr_t*) mem_end - (next_block + 1));
		next_block -> next_block = NULL;
		next_block -> is_free = TRUE;
		first_block -> next_block = next_block;
		next_block -> prev_block = first_block;
	} else if ((next_block + 2) <= last_block) {
		next_block -> block_size = last_block - next_block;
		next_block -> is_free = TRUE;
		next_block -> next_block = last_block;
		last_block -> prev_block = next_block;
		next_block -> prev_block = first_block;
		first_block -> next_block = next_block;
		memset(next_block + 1, 0, next_block -> block_size * BLOCK_SIZE);
	} else {
		// If there is no space for the next free block then consume it
		first_block -> block_size = last_block - (first_block + 1);
		first_block -> next_block = last_block;
		last_block -> prev_block = first_block;
	}

	return;
}

void* calloc(unsigned int size, unsigned int nmemb) {
	const unsigned int tot_size = size * nmemb;
	const unsigned int block_size = tot_size / BLOCK_SIZE + 1 - (tot_size % BLOCK_SIZE == 0);

	block_hdr_t* curr_block = (block_hdr_t*) mem_base;
	while (TRUE) {
		if (curr_block -> is_free && (curr_block -> block_size >= block_size)) break;
		else if (curr_block -> next_block == NULL) return NULL;
		curr_block = curr_block -> next_block;
	}

	curr_block -> is_free = FALSE;
	curr_block -> block_size = block_size;

	insert_next_block(curr_block, curr_block -> next_block);
	memset(curr_block + 1, 0, curr_block -> block_size * BLOCK_SIZE);

	return (curr_block + 1);
}

void* extend(void* ptr, unsigned int size) {
	block_hdr_t* block = (block_hdr_t*) ptr - 1;
	block_hdr_t* prev_block = block -> prev_block;
	block_hdr_t* next_block = block -> next_block;
	const unsigned int old_block_size = block -> block_size;
	const unsigned int block_size = size / BLOCK_SIZE + 1 - (size % BLOCK_SIZE == 0);
	if (block -> block_size >= block_size) {
		block -> block_size = block_size;
		insert_next_block(block, next_block);
		return ptr;
	}

	block_hdr_t* first_block = block;
	block_hdr_t* last_block = next_block;
	if (prev_block -> is_free) first_block = prev_block;
	if (next_block -> is_free) last_block = next_block -> next_block;

	const unsigned int extended_size = last_block - (first_block + 1);
	if (extended_size < block_size) return NULL;

	first_block -> block_size = block_size + 1;
	first_block -> is_free = FALSE;

	if (first_block != block) memcpy(first_block + 1, block + 1, old_block_size * BLOCK_SIZE);
	insert_next_block(first_block, last_block);
	memset(first_block + old_block_size + 1, 0, (first_block -> block_size - old_block_size) * BLOCK_SIZE);

	return (first_block + 1);
}

void free(void* ptr) {
	if (((block_hdr_t*) ptr - 1) < (block_hdr_t*) mem_base || ptr > mem_end) return;
	block_hdr_t* block = (block_hdr_t*) ptr - 1;

	block_hdr_t* first_block = block;
	block_hdr_t* last_block = block -> next_block;
	if (block -> prev_block -> is_free) first_block = block -> prev_block;
	if (block -> next_block -> is_free) last_block = block -> next_block -> next_block;

	first_block -> next_block = last_block;
	last_block -> prev_block = first_block;
	first_block -> is_free = TRUE;
	first_block -> block_size = last_block - (first_block + 1);
	memset(first_block + 1, 0, (first_block -> block_size) * BLOCK_SIZE);

	return;
}

void* realloc(void* old_ptr, unsigned int size, unsigned int nmemb) {
	if (old_ptr == NULL) return calloc(size, nmemb);
	else if (((block_hdr_t*) old_ptr - 1) < (block_hdr_t*) mem_base || old_ptr > mem_end) return NULL;

	void* ptr = calloc(size, nmemb);
	if (ptr == NULL) {
		if ((ptr = extend(old_ptr, size * nmemb))) return ptr;
		free(old_ptr);
		return NULL;
	}

	const unsigned int old_size = ((block_hdr_t*) old_ptr - 1) -> block_size * BLOCK_SIZE;
	memcpy(ptr, old_ptr, old_size);
	free(old_ptr);

	return ptr;
}

void allocator_init(void) {
	const unsigned int mem_size = (unsigned int) (mem_end - mem_base);
	memset((void*) mem_base, 0, mem_size);
	block_hdr_t* first_block = (block_hdr_t*) mem_base;
	first_block -> block_size = mem_size / BLOCK_SIZE - 1;
	first_block -> next_block = NULL;
	first_block -> prev_block = NULL;
	first_block -> is_free = TRUE;
	return;
}

#endif //_ALLOC_H_
