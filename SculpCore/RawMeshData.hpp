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
#include "TrianMeshModel.hpp"

#pragma GCC visibility push(default)


class RawMeshData
{
public:
    
    RawMeshData(std::unique_ptr<Shapr3D::IMesh>&& mesh);
    
    void updateBuffers();

    const float* lowLevelVertices() const;
    const float* lowLevelNormals() const;
    const float* lowLevelColors() const;
    const float* lowLevelVertexColors() const;
    const uint32_t* lowLevelFaceIds() const;

    uint64_t indicesCount() const;
    
    const std::vector<Shapr3D::Vec3>& vertices() const {
        return _originalVertices;
    }

    void changeColorForFace(uint32_t faceId);
    void changeColorForVertex(uint32_t vertexIndex);
    void moveFaceBy(uint32_t faceid, float offset);
    
    uint32_t vertexCount() const;
    
private:
    
    using ShTrianMeshModel = std::shared_ptr<Shapr3D::TrianMeshModel<Shapr3D::DataPool>>;
    
    std::vector<ShTrianMeshModel>   _tmeshes;
    
    std::vector<Shapr3D::GeoTypes::Vec3> _vertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _originalVertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _normals;
    std::vector<vector_float4>          _face_сolors;
    std::vector<vector_float4>          _vertex_colors;
    std::vector<uint32_t>               _faces_ids;

};

#pragma GCC visibility pop
#endif /* RawMeshData_hpp */
