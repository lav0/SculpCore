//
//  RawMeshData.cpp
//  SculpCore
//
//  Created by Andrey on 30.03.2021.
//

#include "RawMeshData.hpp"

RawMeshData::RawMeshData(std::unique_ptr<Shapr3D::IMesh>&& mesh)
: _mesh(std::move(mesh))
{
    _mesh->triangulated_faces();

    updateBuffers();
}

void RawMeshData::updateBuffers()
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
}

const float* RawMeshData::lowLevelVertices() const
{
    return reinterpret_cast<const float*>(_vertices.data());
}
const float* RawMeshData::lowLevelNormals() const
{
    return reinterpret_cast<const float*>(_normals.data());
}
const float* RawMeshData::lowLevelColors() const
{
    return reinterpret_cast<const float*>(_colors.data());
}

const uint32_t* RawMeshData::lowLevelFaceIds() const
{
    return _faces_ids.data();
}

uint64_t RawMeshData::indicesCount() const
{
    return _vertices.size();
}

void RawMeshData::changeColorForFace(uint32_t faceId)
{
    const std::shared_ptr<Shapr3D::GeoTypes::Face>& first_face = _mesh->triangulated_faces()[faceId];
    
    Shapr3D::GeoTypes::Vec4 new_color = {0.9, 0.2, 0.2, 1.0};
    _mesh->changeColorFor(first_face, new_color);
    
    updateBuffers();
}
void RawMeshData::changeVertexColor(uint32_t vertexIndex)
{
    vector_float4 color = {0.05f, 0.9f, 0.05f};
    _colors[vertexIndex] = color;
    _colors[vertexIndex+1] = color;
    _colors[vertexIndex+2] = color;
    
    updateBuffers();
}

uint32_t RawMeshData::vertexCount() const { return _vertices.size(); }
