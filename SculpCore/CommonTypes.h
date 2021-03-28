//
//  CommonTypes.h
//  Shapr3D HomeTest
//
//  Created by Andrey on 09.11.2020.
//

#ifndef CommonTypes_h
#define CommonTypes_h

#pragma GCC visibility push(default)

#include <algorithm>
#include <limits>

namespace Shapr3D {

constexpr size_t BLANK_FACE_ENTRY = std::numeric_limits<size_t>::max();

struct FaceVertex
{
    size_t v=0;
    size_t vt=BLANK_FACE_ENTRY;
    size_t vn=BLANK_FACE_ENTRY;
};

struct Edge
{
    Edge(size_t a, size_t b) : v0(a), v1(b) {}
    
    bool operator==(const Edge& other) const {
        return (v0 == other.v0 && v1 == other.v1) || (v0 == other.v1 && v1 == other.v0);
    }
    
    size_t v0;
    size_t v1;
};

struct EdgeHasher
{
    size_t operator()(const Edge& edge) const {
        return (std::min(edge.v0, edge.v1) << 32) + std::max(edge.v0, edge.v1);
    }
};

}

#pragma GCC visibility pop
#endif /* CommonTypes_h */

