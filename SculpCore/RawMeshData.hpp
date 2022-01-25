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
#include "Node.hpp"

#pragma GCC visibility push(default)

struct NodeLoadInfo
{
    std::string _path;
    std::string _name;
    
    NodeLoadInfo(std::string path, std::string name)
    : _path(path), _name(name) {}
};

class RawMeshData
{
public:
    
    RawMeshData(const std::vector<NodeLoadInfo>& nodes_infos);
    
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
    
    void setPosition(const vector_float3& newValue) {
        _nodesp.front()->setPosition(newValue);
    }
    vector_float3 positionOf(uint32_t index) const {
        return _nodesp[index]->position();
    }
    matrix_float4x4 transformOf(uint32_t index) const {
        return _nodesp[index]->transform();
    }
    
private:
    
    using NodeSp = std::shared_ptr<Shapr3D::Node>;
    
    std::vector<NodeSp>                  _nodesp;
    std::map<uint32_t, size_t>           _offset2nodeIndex;
    
    std::vector<Shapr3D::GeoTypes::Vec3> _vertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _originalVertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _normals;
    std::vector<vector_float4>          _face_сolors;
    std::vector<vector_float4>          _vertex_colors;
    std::vector<uint32_t>               _faces_ids;

};

#pragma GCC visibility pop
#endif /* RawMeshData_hpp */
