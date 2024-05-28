#pragma once

#ifdef GOLEM_ENGINE_EXPORTS
#define GOLEM_ENGINE_API __declspec(dllexport)
#else
#define GOLEM_ENGINE_API __declspec(dllimport)
#endif