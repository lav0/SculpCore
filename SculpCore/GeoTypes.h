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
    using Vec4 = boost::qvm::vec<float, 4>;
    using Mat4x4 = boost::qvm::mat<float,4,4>;
    
    constexpr Vec4 DEFAULT_COLOR = {0.2, 0.2, 0.95, 1.0};
}

#endif /* GeoTypes_h */
