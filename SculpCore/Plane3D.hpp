//
//  Plane3D.hpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 30.11.2020.
//

#ifndef Plane3D_hpp
#define Plane3D_hpp

#include <stdio.h>
#include <boost/qvm/vec.hpp>
#include "IMesh.h"

namespace Shapr3D {

class Plane3D
{
public:
    
    Plane3D(const std::vector<GeoTypes::Vec3>& face);
    
private:
    
    boost::qvm::vec<float, 3> _normal;
    float                     _free_coef;
    
};

}

#endif /* Plane3D_hpp */
