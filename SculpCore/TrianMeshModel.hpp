//
//  TrianMeshModel.hpp
//  SculpCore
//
//  Created by Andrey on 24.01.2022.
//

#ifndef TrianMeshModel_hpp
#define TrianMeshModel_hpp

#include "MeshModel.hpp"

namespace Shapr3D {

template <typename T>
class TrianMeshModel : public IMesh
{
public:
    
    TrianMeshModel(const std::vector<Vec3>& vertices,
                   const std::vector<Vec3>& texture_vertices,
                   const std::vector<Vec3>& normals,
                   const std::vector<Face>& faces,
                   uint32_t face_id_offset)
    : _stored_mesh(std::make_shared<MeshModel<T>>(vertices, texture_vertices, normals, faces))// MeshModel<T>()
    , _face_id_offset(face_id_offset)
    {
        postInit();
    }
    
    TrianMeshModel(const std::shared_ptr<IMesh>& mesh_model,
                   uint32_t face_id_offset
                   )
    : _stored_mesh(mesh_model)
    , _face_id_offset(face_id_offset)
    {
        postInit();
    }
    
    void postInit() {
        auto& ofaces = _stored_mesh->faces(); // original faces
        for (size_t idx=0; idx<ofaces.size(); ++idx) {
            _faces_2_id[ofaces[idx]] = idx + _face_id_offset;
        }
        
        triangulated_faces();
    }
    
    const std::vector<std::shared_ptr<Face>>& faces() const override { return _triangulated_faces; }
    
    bool getFaceId(const std::shared_ptr<GeoTypes::Face>& face, uint32_t& faceid_out) const override;
    const Vec4& getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const override;
    
    bool changeColorFor(const uint32_t& faceid, const Vec4& new_color) override;
    bool moveAlongNormal(const uint32_t& faceid, const float offset) override;
    
    //
    // go through methods
    //
    float surfaceArea() const override { return _stored_mesh->surfaceArea(); }
    float volume() const override { return _stored_mesh->volume(); }
    bool isPointInside(const Vec3& point) const override  { return _stored_mesh->isPointInside(point); }
    bool transform(const boost::qvm::mat<float,4,4> trs) override { return _stored_mesh->transform(trs); }
    
    const std::vector<Vec3>& vertices() const override { return _stored_mesh->vertices(); }
    const std::vector<Vec3>& normals() const override { return _stored_mesh->normals(); }
    const std::vector<Vec3>& texture_vertices() const override { return _stored_mesh->texture_vertices(); }
    const std::vector<uint32_t>& indices() const override { return _stored_mesh->indices(); }
    Vec3 getFaceNormal(const Face& face) const override { return _stored_mesh->getFaceNormal(face); } // tmp : qqq ??
    const Vec4& getVertexColor(size_t vertex_index) const override { return _stored_mesh->getVertexColor(vertex_index); }
    
private:
    
    const std::vector<std::shared_ptr<Face>>& triangulated_faces();
    
private:
    
    uint32_t _face_id_offset;
    std::vector<std::shared_ptr<Face>> _triangulated_faces;
    std::map<std::shared_ptr<Face>, std::shared_ptr<Face>> _triungulated_face_2_original;
    std::map<std::shared_ptr<Face>, size_t> _faces_2_id;
    
    std::shared_ptr<IMesh> _stored_mesh; // we'd like to keep the origal mesh to make sure it's deallocated somewhere eslse
};

}
#endif /* TrianMeshModel_hpp */
