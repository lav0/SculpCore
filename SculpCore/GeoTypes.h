//
//  GeoTypes.h
//  SculpCore
//
//  Created by Andrey on 28.03.2021.
//

#ifndef GeoTypes_h
#define GeoTypes_h

#include <boost/qvm/vec.hpp>
#include <boost/qvm/mat.hpp>
#include <vector>
#include "CommonTypes.h"

namespace Shapr3D::GeoTypes {
    using Face = std::vector<FaceVertex>;
    using Vec3 = boost::qvm::vec<float, 3>;
    using Mat4x4 = boost::qvm::mat<float,4,4>;
}

#endif /* GeoTypes_h */
