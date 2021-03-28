//
//  CommonFunctions.hpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 15.11.2020.
//

#ifndef CommonFunctions_hpp
#define CommonFunctions_hpp

#pragma GCC visibility push(default)

#include "GeoTypes.h"

namespace Shapr3D::CommonFunctions {

bool isFaceInsideOtherFace(const Shapr3D::GeoTypes::Face& inner_face, const Shapr3D::GeoTypes::Face& container_face);

bool operator==(const Shapr3D::GeoTypes::Face& a, const Shapr3D::GeoTypes::Face& b);

float getArea(const Shapr3D::GeoTypes::Face& face, const std::vector<Shapr3D::GeoTypes::Vec3>& vertices);

}

#pragma GCC visibility pop
#endif /* CommonFunctions_hpp */
