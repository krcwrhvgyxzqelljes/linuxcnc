#ifndef VECTOR_H
#define VECTOR_H



#ifdef __cplusplus

#include "segment.h"
#include "vector"

class vector
{
public:
    vector();

    std::vector<segment> segvec;
};

#else
typedef struct tp_vector tp_vector;
#endif // cplusplus

#endif // VECTOR_H
