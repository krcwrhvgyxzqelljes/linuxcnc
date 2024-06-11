#ifndef PRINT_SECTION_H
#define PRINT_SECTION_H

#include "def_section.h"

// Define the print macro
#if COMP_DEBUG
    #define print(...) printf(__VA_ARGS__)
#else
    #define print(...) // No operation
#endif

#endif // PRINT_SECTION_H
