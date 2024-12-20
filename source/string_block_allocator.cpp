#include "string_block_allocator.hpp"
#include "bind.hpp"

StringBlockAllocator::StringBlockAllocator(std::size_t _initial_string_len, int _alloc_size) {
	initial_string_len = _initial_string_len;
	if (_alloc_size == -1) {
		alloc_size = _initial_string_len;
	}
	else {
		alloc_size = _alloc_size;
	}
}

StringBlockAllocator::~StringBlockAllocator() {
	for (std::size_t i = 0; i < string_blocks.Length(); i++) {
		delete string_blocks[i];
	}
	string_blocks.Clear();
}

void StringBlockAllocator::Create(int unk) {
	string_blocks.CLNonMacroCreate(0x80, 0x40, std::numeric_limits<int>::max());
	StringBlock* block = new StringBlock();
	block->unk = unk;
	char* buffer = reinterpret_cast<char*>(calloc(1, initial_string_len));
	block->end = buffer;
	block->start = buffer;
	string_blocks.CLAddItem(block);
}

char* StringBlockAllocator::StringBlockAllocator_AllocStringByLength(int length, int unk)
{
	for (int block_index = 0; block_index < string_blocks.Length(); block_index++) {
		StringBlock* current_block = string_blocks[block_index];

		size_t available_space = 0;

		if (block_index == 0) {
			if (current_block->unk == unk) {
				available_space = initial_string_len;
			}
			else {
				continue;
			}
		}
		else if (current_block->unk == unk) {
			available_space = alloc_size;
		}
		else {
			continue;
		}

		if (length + 1 <= static_cast<int>((available_space - (current_block->end - current_block->start)))) {
			current_block->ref_count += 1;
			char* allocated_string = current_block->end;
			current_block->end += length + 1;
			return allocated_string;
		}
	}

	StringBlock* new_block = new StringBlock();
	new_block->unk = unk;

	size_t block_size = (string_blocks.Length() == 0) ? initial_string_len : alloc_size;
	char* new_memory = reinterpret_cast<char*>(calloc(1, block_size));

	new_block->start = new_memory;
	new_block->end = new_memory;

	string_blocks.CLAddItem(new_block);

	new_block->ref_count += 1;
	char* allocated_string = new_block->end;
	new_block->end += length + 1;

	return allocated_string;
}

char* StringBlockAllocator::StringBlockAllocator_AllocStringByString(char* str, int unk)
{
	return StringBlockAllocator_AllocStringByLength(strlen(str), unk);
}

void StringBlockAllocator::FreeString(char* str) {
	std::size_t index = -1;
	for (std::size_t i = 0; i < string_blocks.Length(); i++) {
		if (string_blocks[i]->start <= str && str < string_blocks[i]->end) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		return;
	}
	string_blocks[index]->ref_count--;
	if (string_blocks[index]->ref_count != 0) {
		return;
	}
	string_blocks[index]->end = string_blocks[index]->start;
}

StringBlockAllocator::StringBlock::StringBlock()
{
	start = nullptr;
	ref_count = 0;
	end = nullptr;
	unk = 0;
}

StringBlockAllocator::StringBlock::~StringBlock()
{
	free(start);
}
