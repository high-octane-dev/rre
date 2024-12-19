#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>

/*
inline auto operator_new = (void* (__cdecl*)(std::size_t))(0x0063eb87);
inline auto operator_delete = (void(__cdecl*)(void*))(0x0063f514);
inline auto _realloc = (void* (__cdecl*)(void*, std::size_t))(0x0063f73a);
*/

#define UNIMPLEMENTED(ptr) constexpr static std::uint32_t addr = ##ptr; __asm jmp addr