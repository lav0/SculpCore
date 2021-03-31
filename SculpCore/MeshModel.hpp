//
//  MeshModel.hpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 09.11.2020.
//

#ifndef MeshModel_hpp
#define MeshModel_hpp

#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <map>
#include "IMesh.h"

#pragma GCC visibility push(default)


namespace Shapr3D {


using namespace GeoTypes;

struct DataPool
{
    DataPool() = default;
    DataPool(const std::vector<Vec3>& vertices,
             const std::vector<Vec3>& texture_vertices,
             const std::vector<Vec3>& normals)
    : _vertices(vertices)
    , _texture_vertices(texture_vertices)
    , _normals(normals) {}
    
    std::vector<Vec3> _vertices;
    std::vector<Vec3> _texture_vertices;
    std::vector<Vec3> _normals;
    std::vector<uint32_t> _indices;
};

struct Line3D
{
    Vec3 point;
    Vec3 vector;
};

template <typename T>
class MeshModel : public IMesh,
                  public T
{
    using ParentDataPool = T;
    
public:

    MeshModel(const std::vector<Vec3>& vertices,
              const std::vector<Vec3>& texture_vertices,
              const std::vector<Vec3>& normals,
              const std::vector<Face>& faces);
    
    float surfaceArea() const override;
    float volume() const override;
    bool isPointInside(const Vec3& point) const override;
    bool transform(const boost::qvm::mat<float,4,4> trs) override;
    
    const std::vector<std::shared_ptr<Face>>& faces() const override { return _faces; }
    const std::vector<Vec3>& vertices() const override { return ParentDataPool::_vertices; }
    const std::vector<Vec3>& normals() const override { return ParentDataPool::_normals; }
    const std::vector<Vec3>& texture_vertices() const override { return ParentDataPool::_texture_vertices; }
    const std::vector<uint32_t>& indices() const override { return ParentDataPool::_indices; }
    Vec3 getFaceNormal(const Face& face) const override;
    const Vec4& getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const override;
    const Vec4& getVertexColor(size_t vertex_index) const override;
    bool changeColorFor(const std::shared_ptr<GeoTypes::Face>& face, const Vec4& new_color) override;
    
    const std::vector<std::shared_ptr<Face>>& triangulated_faces() override;
    
private:
    
    void fillInHelperContainer(const std::vector<Face>& faces);
    
    Vec3 getFaceNormal(const Face& face, size_t last_index) const;
    GeoTypes::Vec3 projectPointOnFacePlane(const GeoTypes::Vec3& point, const Face& face) const;
    GeoTypes::Vec3 findIntersection(const Shapr3D::Line3D& line, const Face& face) const;
    GeoTypes::Vec3 faceGravityCentre(const Face& face) const;
    float getDistanceToFaceFrom(const Vec3& point, const Face& face) const;
    bool isPointInsideFace(const Vec3& point, const Face& face) const;
    
private:
    
    std::vector<std::shared_ptr<Face>> _faces;
    std::vector<std::shared_ptr<Face>> _triangulated_faces;
    std::map<std::shared_ptr<Face>, std::shared_ptr<Face>> _triungulated_face_2_original;
    
    std::map<std::shared_ptr<Face>, Vec4> _face_colors;
    std::vector<Vec4>                     _vertex_colors;
};


}

#pragma GCC visibility pop
#endif /* MeshModel_hpp */
