#pragma once
#include "Debug/log.h"

#ifdef _DEBUG
#define Assert(cond) { if (!(cond)) { Log::Print("Assertion failed"); __debugbreak(); }  else (void)0; }
#else
#define Assert(cond) { !(cond) ? throw std::runtime_error("Assertion failed") : (void)0; }
#endif

