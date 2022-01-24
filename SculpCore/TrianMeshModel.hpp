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
class TrianMeshModel : public MeshModel<T>
{
public:
    
    TrianMeshModel(const std::vector<Vec3>& vertices,
                   const std::vector<Vec3>& texture_vertices,
                   const std::vector<Vec3>& normals,
                   const std::vector<Face>& faces,
                   uint32_t face_id_offset)
    : MeshModel<T>(vertices, texture_vertices, normals, faces)
    , _face_id_offset(face_id_offset)
    {
        auto& ofaces = MeshModel<T>::_mesh_faces; // original faces
        for (size_t idx=0; idx<ofaces.size(); ++idx) {
            _faces_2_id[ofaces[idx]] = idx + _face_id_offset;
        }
        
        triangulated_faces();
    }
    
    TrianMeshModel(const MeshModel<T>& mesh_model,
                   uint32_t face_id_offset)
    : MeshModel<T>(mesh_model)
    , _face_id_offset(face_id_offset)
    {
        assert(false); // not yet
    }
    
    const std::vector<std::shared_ptr<Face>>& faces() const override { return _triangulated_faces; }
    
    bool getFaceId(const std::shared_ptr<GeoTypes::Face>& face, uint32_t& faceid_out) const override;
    const Vec4& getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const override;
    
    bool changeColorFor(const uint32_t& faceid, const Vec4& new_color) override;
    bool moveAlongNormal(const uint32_t& faceid, const float offset) override;
    
private:
    
    const std::vector<std::shared_ptr<Face>>& triangulated_faces();
    
private:
    
    uint32_t _face_id_offset;
    std::vector<std::shared_ptr<Face>> _triangulated_faces;
    std::map<std::shared_ptr<Face>, std::shared_ptr<Face>> _triungulated_face_2_original;
    std::map<std::shared_ptr<Face>, size_t> _faces_2_id;
    
    
};

}
#endif /* TrianMeshModel_hpp */
