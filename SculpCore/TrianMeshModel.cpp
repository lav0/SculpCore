//
//  TrianMeshModel.cpp
//  SculpCore
//
//  Created by Andrey on 24.01.2022.
//

#include "TrianMeshModel.hpp"

using namespace Shapr3D;


template<typename T>
const std::vector<std::shared_ptr<Face>>& TrianMeshModel<T>::triangulated_faces()
{
    if (_triangulated_faces.empty())
    {
        _triungulated_face_2_original.clear();
        
        const auto& origin_faces = _stored_mesh->faces();
        
        bool mesh_has_changes = false;
        
        for (auto& p_origin_face : origin_faces)
        {
            assert(p_origin_face->size() >= 3);
            
            if (p_origin_face->size() == 3)
            {
                _triangulated_faces.push_back(p_origin_face);
                _triungulated_face_2_original.emplace(p_origin_face, p_origin_face);
                continue;
            }
            
            size_t faceid = _faces_2_id[p_origin_face];
            
            auto origin_face = *p_origin_face;
            
            auto pivot_vx = origin_face[0];
            auto secon_vx = origin_face[1];
            
            for (size_t i=2; i<origin_face.size(); ++i)
            {
                auto third_vx = origin_face[i];
                
                Face new_face(3);
                new_face[0] = pivot_vx;
                new_face[1] = secon_vx;
                new_face[2] = third_vx;
                
                _triangulated_faces.push_back(std::make_shared<Face>(new_face));
                _triungulated_face_2_original.emplace(_triangulated_faces.back(),
                                                      p_origin_face
                                                      );
                _faces_2_id[_triangulated_faces.back()] = faceid;
                
                secon_vx = third_vx;
                
                if (i == 3)
                    mesh_has_changes = true;
            }
        }
    }
    
    return _triangulated_faces;
}


template<typename T>
bool TrianMeshModel<T>::getFaceId(const std::shared_ptr<GeoTypes::Face>& face, uint32_t& faceid_out) const
{
    auto find_iter = _faces_2_id.find(face);
    
    if (find_iter != _faces_2_id.end()) {
        faceid_out = find_iter->second;
        return true;
    }
    
    return false;
}

template<typename T>
const Vec4& TrianMeshModel<T>::getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const
{
    // TODO: use bool as return
    auto triface_2_origface = _triungulated_face_2_original.find(face);
    return _stored_mesh->getFaceColor(triface_2_origface->second);
}

template<typename T>
bool TrianMeshModel<T>::changeColorFor(const uint32_t& faceid, const Vec4& new_color)
{
    return _stored_mesh->changeColorFor(faceid - _face_id_offset, new_color);
}

template<typename T>
bool TrianMeshModel<T>::moveAlongNormal(const uint32_t& faceid, const float offset)
{
    return _stored_mesh->moveAlongNormal(faceid - _face_id_offset, offset);
}


namespace Shapr3D {
    template class TrianMeshModel<DataPool>;
}
