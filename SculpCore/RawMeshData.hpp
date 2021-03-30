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
    
    RawMeshData(std::unique_ptr<Shapr3D::IMesh>&& mesh);
    
    void updateBuffers();

    const float* lowLevelVertices() const;
    const float* lowLevelNormals() const;
    const float* lowLevelColors() const;
    const uint32_t* lowLevelFaceIds() const;

    uint64_t indicesCount() const;

    void changeColorForFace(uint32_t faceId);
    void changeVertexColor(uint32_t vertexIndex);
    
    uint32_t vertexCount() const;
    
private:
    
    std::unique_ptr<Shapr3D::IMesh> _mesh;
    
    std::vector<Shapr3D::GeoTypes::Vec3> _vertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _normals;
    std::vector<vector_float4>        _colors;
    std::vector<uint32_t>               _faces_ids;

};

#pragma GCC visibility pop
#endif /* RawMeshData_hpp */
