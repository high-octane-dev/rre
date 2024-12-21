#include <corecrt_malloc.h>
#include <cstddef>
#include "block_allocator.hpp"

// OFFSET: 0x0053db80
BlockAllocator::BlockAllocator(int _used_size_per_block, int _block_size_bytes) : BaseObject() {
    used_size_per_block = _used_size_per_block;
    alignment_unused = 0;
    unk_unused = 0;
    big_blocks = nullptr;
    last_block = nullptr;
    free_block = nullptr;
    small_blocks = nullptr;
    unk_callback = nullptr;
    block_size_bytes = _block_size_bytes;
    block_data_offset = sizeof(Block*);
    max_extra_uses_per_block = (_block_size_bytes - _used_size_per_block) / _used_size_per_block;
    uses_in_current_block = (_block_size_bytes - _used_size_per_block) / _used_size_per_block;
    free_memory_not_new_block = nullptr;
}

// OFFSET: 0x0053dca0
BlockAllocator::~BlockAllocator() {
    Block* block = big_blocks;
    while (block != nullptr) {
        Block* next_block = block->next;
        delete block;
        block = next_block;
    }

    block = small_blocks;
    while (block != nullptr) {
        Block* next_block = block->next;
        delete block;
        block = next_block;
    }

    big_blocks = nullptr;
    last_block = nullptr;
    free_block = nullptr;
    small_blocks = nullptr;
    uses_in_current_block = this->max_extra_uses_per_block;
}

// OFFSET: 0x005a4d10
void* BlockAllocator::AllocBlock(int* allocated_new_block) {
    Block* block;
    std::uint8_t* out_buf;

    if (allocated_new_block != nullptr) {
        *allocated_new_block = 0;
    }
    if (this->uses_in_current_block < this->max_extra_uses_per_block) {
        out_buf = reinterpret_cast<std::uint8_t*>(this->free_memory_not_new_block);
        this->free_memory_not_new_block = this->used_size_per_block + out_buf;
        this->uses_in_current_block = this->uses_in_current_block + 1;
        return out_buf;
    }
    block = this->free_block;
    if (block != nullptr) {
        this->free_block = block->next;
        return block;
    }
    if ((allocated_new_block != nullptr) && (this->big_blocks != nullptr)) {
        *allocated_new_block = 1;
    }
    return AllocBlockComplex();
}

// OFFSET: 0x0053dc00
int BlockAllocator::AllocBigBlock() {
    Block* block = nullptr;
    if (this->alignment_unused == 0) {
        block = reinterpret_cast<Block*>(malloc(this->block_size_bytes + this->block_data_offset));
        if (block == nullptr) {
            return 0;
        }
    }
    else {
        // on non-windows platforms, this should be a memalign
        block = reinterpret_cast<Block*>(malloc(this->block_size_bytes + this->block_data_offset));
        if (block == nullptr) {
            return 0;
        }
    }
    if (this->unk_callback != nullptr) {
        for (std::size_t i = 0; i < max_extra_uses_per_block; i++) {
            void* memory = reinterpret_cast<std::uint8_t*>(block) + block_data_offset + used_size_per_block * i;
            (*unk_callback)(memory);
        }
    }
    if (this->big_blocks != nullptr) {
        this->last_block->next = block;
        this->last_block = block;
        block->next = nullptr;
        return 1;
    }
    this->big_blocks = block;
    this->last_block = block;
    block->next = nullptr;
    return 1;
}

// OFFSET: 0x00574f20
void* BlockAllocator::AllocBlockComplex() {
    if (this->small_blocks == nullptr) {
        if (AllocBigBlock() == 0) {
            return 0;
        }
    }
    else {
        if (this->big_blocks == nullptr) {
            this->big_blocks = this->small_blocks;
        }
        else {
            this->last_block->next = this->small_blocks;
        }
        auto temp = this->small_blocks;
        this->last_block = temp;
        this->small_blocks = temp->next;
        temp->next = nullptr;
    }
    std::uint8_t* block_data = reinterpret_cast<std::uint8_t*>(this->last_block) + this->block_data_offset;
    this->uses_in_current_block = 1;
    this->free_memory_not_new_block = this->used_size_per_block + block_data;
    return block_data;
}

// OFFSET: INLINE
void BlockAllocator::FreeBlock(void* memory)
{
    if (4 < used_size_per_block) {
        // WHAT IN THE UNSAFE IS THIS?!?! AAAAAAHHHHHHH
        auto* block = reinterpret_cast<Block*>(memory);
        block->next = free_block;
        free_block = block;
    }
}
