#pragma once
#include <iostream>
#include <assert.h>


#define ASSERT(condition, message) do { } while (false);

/*
std::cerr << "Assertion `" #condition "` failed in " << __FUNCTION__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \

					   std::exit(EXIT_FAILURE); \
*/