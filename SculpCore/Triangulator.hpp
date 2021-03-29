//
//  Triangulator.hpp
//  SculpCore
//
//  Created by Andrey on 29.03.2021.
//

#ifndef Triangulator_hpp
#define Triangulator_hpp

#include <stdio.h>
#include <memory>
#include "IMesh.h"


#pragma GCC visibility push(default)

namespace Shapr3D {
class Triangulator
{
    using V = GeoTypes::Vec3;
    using F = GeoTypes::Face;
    
public:
    
    Triangulator(const IMesh& inmesh);
    
    std::unique_ptr<IMesh> triangulated_mesh() const;
    
    static bool triangulate(const IMesh& mesh, std::vector<Face>& out_faces);
    
private:
    
    bool triangulate(std::vector<F>& faces) const;
    
private:
    
    const IMesh& _inmesh;
    
};
}

#pragma GCC visibility pop
#endif /* Triangulator_hpp */
