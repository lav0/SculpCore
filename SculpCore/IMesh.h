//
//  IMesh.h
//  Shapr3D_HomeTest
//
//  Created by Andrey on 14.11.2020.
//

#ifndef IMesh_h
#define IMesh_h

#include <vector>
#include "CommonTypes.h"
#include "GeoTypes.h"

#pragma GCC visibility push(default)


namespace Shapr3D {

using namespace GeoTypes;
class IMesh
{
public:
    
public:
    
    virtual ~IMesh() = default;
    
    virtual float surfaceArea() const = 0;
    virtual float volume() const = 0;
    virtual bool isPointInside(const Vec3& point) const = 0;
    virtual bool transform(const Mat4x4 trs) = 0;
    
    virtual const std::vector<std::shared_ptr<std::vector<FaceVertex>>>& faces() const = 0;
    virtual const std::vector<Vec3>&    vertices() const = 0;
    virtual const std::vector<Vec3>&    normals() const = 0;
    virtual const std::vector<Vec3>&    texture_vertices() const = 0;
    virtual const std::vector<uint32_t>& indices() const = 0;
    virtual Vec3 getFaceNormal(const std::vector<FaceVertex>& face) const = 0;
    virtual const Vec4& getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const = 0;
    virtual bool changeColorFor(const std::shared_ptr<GeoTypes::Face>& face, const Vec4& new_color) = 0;
    
    virtual const std::vector<std::shared_ptr<std::vector<FaceVertex>>>& triangulated_faces() = 0;
    
};



}

#pragma GCC visibility pop
#endif /* IMesh_h */
