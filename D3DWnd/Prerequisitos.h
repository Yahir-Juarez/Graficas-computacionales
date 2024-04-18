#pragma once

#include <memory>
#include <vector>
#include <string>

using uint32 = unsigned int;
using int32 = int;

using Wstring = std::wstring;
using Path = std::string;
using String = std::string;


template<class T>
using Vector = std::vector<T>;

template<typename T>
using SPtr = std::shared_ptr<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

#define SAFE_RELEASE(x) {if(x){x->Release(); x = nullptr;} }