#ifndef ROPTOOL_TYPES_H_
#define ROPTOOL_TYPES_H_

// std
#include <cstdint>

// boost
#include <boost/variant.hpp>

// define integer types
typedef std::uint64_t u64;
typedef std::uint32_t u32;
typedef std::uint16_t u16;
typedef std::uint8_t u8;

typedef std::int64_t s64;
typedef std::int32_t s32;
typedef std::int16_t s16;
typedef std::int8_t s8;

// variant used to hold all unsigned integer types
typedef boost::variant<u64, u32, u16, u8> uint_var;

// enum to describe available word lengths
enum WordLength
{
    BYTE_LENGTH = 8,
    WORD_LENGTH = 16,
    DWORD_LENGTH = 32,
    QWORD_LENGTH = 64
};

#endif //ROPTOOL_TYPES_H_
