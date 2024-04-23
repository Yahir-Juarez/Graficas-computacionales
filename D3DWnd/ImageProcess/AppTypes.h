#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>
#include <string>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using Byte = std::byte;

template<typename T>
using Vector = std::vector<T>;

using String = std::string;
using WString = std::wstring;

using Path = String;
