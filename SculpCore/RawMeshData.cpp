//
//  RawMeshData.cpp
//  SculpCore
//
//  Created by Andrey on 30.03.2021.
//

#include "RawMeshData.hpp"

using namespace Shapr3D;

RawMeshData::RawMeshData(std::unique_ptr<IMesh>&& mesh)
{
    std::shared_ptr<IMesh> omesh = std::move(mesh);
    _tmeshes.push_back(std::make_shared< TrianMeshModel<DataPool> >(omesh, 1));
    
    updateBuffers();
}

void RawMeshData::updateBuffers()
{
    _normals.clear();
    _vertices.clear();
    _face_сolors.clear();
    _vertex_colors.clear();
    _faces_ids.clear();
    
    for (auto& mesh : _tmeshes)
    {
        auto faces = mesh->faces();
        auto verts = mesh->vertices();
        
        for (auto& f : faces)
        {
            auto face_color = mesh->getFaceColor(f);
            vector_float4 color = {0, 0, 0, 1.f};
            color.x = X(face_color);
            color.y = Y(face_color);
            color.z = Z(face_color);
            
            uint32_t face_id = 0;
            if (!mesh->getFaceId(f, face_id)) {
                assert(false);
                continue;
            }
            
            for (auto& fv : *f)
            {
                auto& n = mesh->normals()[fv.vn];
                auto& v = mesh->vertices()[fv.v];
                
                _normals.push_back(n);
                _vertices.push_back(v);
                _face_сolors.push_back(color);
                _faces_ids.push_back(face_id);
            }
        }
        
        for (size_t vi=0; vi < vertices().size(); ++vi)
        {
            Vec4 vc = mesh->getVertexColor(vi);
            
            vector_float4 vertex_color = {0, 0, 0, 1.f};
            vertex_color.x = X(vc);
            vertex_color.y = Y(vc);
            vertex_color.z = Z(vc);
            vertex_color.w = W(vc);
            
            _vertex_colors.push_back(vertex_color);
        }
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
    return reinterpret_cast<const float*>(_face_сolors.data());
}
const float* RawMeshData::lowLevelVertexColors() const
{
    return reinterpret_cast<const float*>(_vertex_colors.data());
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
    Shapr3D::GeoTypes::Vec4 new_color = {0.9, 0.2, 0.2, 1.0};
    _tmeshes.front()->changeColorFor(faceId, new_color);
    
    updateBuffers();
}
void RawMeshData::changeColorForVertex(uint32_t vertexIndex)
{
    vector_float4 color = {0.05f, 0.9f, 0.05f};
    _face_сolors[vertexIndex] = color;
    _face_сolors[vertexIndex+1] = color;
    _face_сolors[vertexIndex+2] = color;
    
    updateBuffers();
}

void RawMeshData::moveFaceBy(uint32_t faceid, float offset)
{
    _tmeshes.front()->moveAlongNormal(faceid, offset);
    
    updateBuffers();
}

uint32_t RawMeshData::vertexCount() const
{
    return static_cast<uint32_t>(_vertices.size());
}
