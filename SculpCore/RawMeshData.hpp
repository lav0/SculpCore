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
#include <limits>
#include <stdio.h>
#include "GeoTypes.h"
#include "Node.hpp"
#include "Utils/IntervalMap.cpp"

#pragma GCC visibility push(default)

struct NodeLoadInfo
{
    std::string _path;
    std::string _name;
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
    
    uint32_t originalVertexCount() const;
    
    void setPosition(size_t nodeIndex, const vector_float3& newValue);
    
    vector_float3 positionOf(size_t nodeIndex) const {
        return _nodesp[nodeIndex]->position();
    }
    matrix_float4x4 transformOf(size_t nodeIndex) const {
        return _nodesp[nodeIndex]->transform();
    }
    
    uint32_t indexCountOf(size_t nodeIndex) const {
        return 3 * _nodesp[nodeIndex]->faceCount();
    }
    uint64_t indexCountOriginal(size_t nodeIndex) const {
        return _nodesp[nodeIndex]->vertexCount();
    }
    uint32_t startVertexOf(size_t nodeIndex) const {
        auto it = _node2vertexStart.find(nodeIndex);
        if (it != _node2vertexStart.end()) {
            return it->second;
        }
        assert(false);
        return 0;
    }
    uint64_t vertexStartOriginal(size_t nodeIndex) const {
        auto it = _node2origVertexStart.find(nodeIndex);
        if (it != _node2origVertexStart.end()) {
            return it->second;
        }
        assert(false);
        return INVALID_VERTEX_COUNT;
    }
    
private:
    
    constexpr static size_t INVALID_NODE_INDEX = std::numeric_limits<size_t>::max();
    constexpr static uint64_t INVALID_VERTEX_COUNT = std::numeric_limits<uint64_t>::max();
    
    using NodeSp = std::shared_ptr<Shapr3D::Node>;
    
    std::vector<NodeSp>                  _nodesp;
    
    // face offsets are used to find a unique triangle being tapped
    // so we need to have unique face ids across all meshes.
    // and here we map face id offsets interval to there respective nodes
    IntervalMap<uint32_t, size_t>        _offset2nodeIndex = IntervalMap<uint32_t, size_t>(INVALID_NODE_INDEX);
    
    std::map<size_t, uint64_t>           _node2origVertexStart;
    std::map<size_t, uint32_t>           _node2vertexStart;
    
    std::vector<Shapr3D::GeoTypes::Vec3> _vertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _originalVertices;
    std::vector<Shapr3D::GeoTypes::Vec3> _normals;
    std::vector<vector_float4>          _face_сolors;
    std::vector<vector_float4>          _vertex_colors;
    std::vector<uint32_t>               _faces_ids;

};

#pragma GCC visibility pop
#endif /* RawMeshData_hpp */
