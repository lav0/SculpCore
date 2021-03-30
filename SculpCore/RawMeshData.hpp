//
//  RawMeshData.hpp
//  SculpCore
//
//  Created by Andrey on 30.03.2021.
//

#ifndef RawMeshData_hpp
#define RawMeshData_hpp

#include <boost/qvm/vec_access.hpp>
#include <simd/simd.h>
#include <stdio.h>
#include "GeoTypes.h"
#include "IMesh.h"

#pragma GCC visibility push(default)


class RawMeshData
{
public:
    
    RawMeshData(std::unique_ptr<Shapr3D::IMesh>&& mesh)
    : _mesh(std::move(mesh))
    {
        _mesh->triangulated_faces();

        _dirty = true;
    }
    
    void updateBuffers()
    {
        _normals.clear();
        _vertices.clear();
        _colors.clear();
        _faces_ids.clear();
        
        auto faces = _mesh->triangulated_faces();
        auto verts = _mesh->vertices();
        
        uint32_t face_id = 1;
        
        for (auto& f : faces)
        {
            auto face_color = _mesh->getFaceColor(f);
            vector_float4 color = {0, 0, 0, 1.f};
            color.x = X(face_color);
            color.y = Y(face_color);
            color.z = Z(face_color);

            for (auto& fv : *f)
            {
                auto& n = _mesh->normals()[fv.vn];
                auto& v = _mesh->vertices()[fv.v];

                _normals.push_back(n);
                _vertices.push_back(v);
                _colors.push_back(color);
                _faces_ids.push_back(face_id);
            }
            
            ++face_id;
        }
        
        _dirty = false;
    }

    const float* lowLevelVertices()
    {
        if (_dirty) {
            updateBuffers();
        }
        
    //    return reinterpret_cast<const float*>(_mesh->vertices().data());
        return reinterpret_cast<const float*>(_vertices.data());
    //    return _instance.vertices().data();
    }
    const float* lowLevelNormals()
    {
        if (_dirty) {
            updateBuffers();
        }
        
    //    return reinterpret_cast<const float*>(_mesh->normals().data());
        return reinterpret_cast<const float*>(_normals.data());
    //    return _instance.normals().data();
    }
    const float* lowLevelColors()
    {
        if (_dirty) {
            updateBuffers();
        }
        
        return reinterpret_cast<const float*>(_colors.data());
    }

    const uint32_t* lowLevelFaceIds()
    {
        if (_dirty) {
            updateBuffers();
        }
        
        return _faces_ids.data();
    }

    uint64_t indicesCount()
    {
        if (_dirty) {
            updateBuffers();
        }
        
        return _vertices.size();
    }

    void changeRandomFaceColor(uint32_t vertexIndex)
    {
        const std::shared_ptr<Shapr3D::GeoTypes::Face>& first_face = _mesh->triangulated_faces()[vertexIndex];
        
        Shapr3D::GeoTypes::Vec4 new_color = {0.9, 0.2, 0.2, 1.0};
        _mesh->changeColorFor(first_face, new_color);
        _dirty = true;
    }
    void changeVertexColor(uint32_t vertexIndex)
    {
        vector_float4 color = {0.05f, 0.9f, 0.05f};
        _colors[vertexIndex] = color;
        _colors[vertexIndex+1] = color;
        _colors[vertexIndex+2] = color;
        _dirty = true;
    }
    
    uint32_t vertexCount() { return _vertices.size(); }
    
private:
    
    std::unique_ptr<Shapr3D::IMesh> _mesh;
    
    std::vector<Shapr3D::GeoTypes::Vec3> _vertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _normals;
    std::vector<vector_float4>        _colors;
    std::vector<uint32_t>               _faces_ids;
    
    bool _dirty;
};

#pragma GCC visibility pop
#endif /* RawMeshData_hpp */
