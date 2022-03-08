//
//  RawMeshData.cpp
//  SculpCore
//
//  Created by Andrey on 30.03.2021.
//

#include "RawMeshData.hpp"
#include "MeshBuilder.cpp"
using namespace Shapr3D;

RawMeshData::RawMeshData(const std::vector<NodeLoadInfo>& nodes_infos)
{
    uint32_t prev_vertex_count = 0;
    uint64_t orig_vertex_count = 0;
    uint32_t scene_offset = 1;
    
    auto newNode = [&prev_vertex_count, &orig_vertex_count, &scene_offset, this](auto node_sp)
    {
        assert(INVALID_NODE_INDEX != _nodesp.size()); // very unlikely to happen
        
        _offset2nodeIndex.assign(scene_offset,
                                 scene_offset + node_sp->faceCount(),
                                 _nodesp.size());
        
        _node2vertexStart[_nodesp.size()] = prev_vertex_count;
        _node2origVertexStart[_nodesp.size()] = orig_vertex_count;
        
        _nodesp.push_back(node_sp);
        
        scene_offset += node_sp->faceCount();
        prev_vertex_count += 3 * node_sp->faceCount();
        orig_vertex_count += node_sp->vertexCount();
    };
    
    for (auto& info : nodes_infos)
    {
        newNode(std::make_shared<Node>(info._name, info._path, scene_offset));
    }
    
//    auto n = std::make_shared<Node>("tri", MeshBuilder::buildTriangle(), scene_offset);
//    newNode(n);
    
    updateBuffers();
}

void RawMeshData::pushBack(const std::shared_ptr<Shapr3D::TrianMeshModel<Shapr3D::DataPool>>& tmesh)
{
    auto faces = tmesh->faces();
    auto verts = tmesh->vertices();
    
    _originalVertices.insert(_originalVertices.end(), verts.begin(), verts.end());
    
    for (auto& f : faces)
    {
        auto face_color = tmesh->getFaceColor(f);
        vector_float4 color = {0, 0, 0, 1.f};
        color.x = X(face_color);
        color.y = Y(face_color);
        color.z = Z(face_color);
        
        uint32_t face_id = 0;
        if (!tmesh->getFaceId(f, face_id)) {
            assert(false);
            continue;
        }
        
        for (auto& fv : *f)
        {
            auto& n = tmesh->normals()[fv.vn];
            auto& v = tmesh->vertices()[fv.v];
            
            _normals.push_back(n);
            _vertices.push_back(v);
            _face_сolors.push_back(color);
            _faces_ids.push_back(face_id);
        }
    }
    
    for (size_t vi=0; vi < tmesh->vertices().size(); ++vi)
    {
        Vec4 vc = tmesh->getVertexColor(vi);
        
        vector_float4 vertex_color = {0, 0, 0, 1.f};
        vertex_color.x = X(vc);
        vertex_color.y = Y(vc);
        vertex_color.z = Z(vc);
        vertex_color.w = W(vc);
        
        _vertex_colors.push_back(vertex_color);
    }
}

void RawMeshData::updateBuffers()
{
    _normals.clear();
    _vertices.clear();
    _originalVertices.clear();
    _face_сolors.clear();
    _vertex_colors.clear();
    _faces_ids.clear();
    
    for (auto& node_sp : _nodesp)
    {
        auto& mesh = node_sp->triangulated_mesh();
        
        pushBack(mesh);
    }
}

const uint64_t RawMeshData::addMesh(std::unique_ptr<IMesh>&& newMesh)
{
    const auto greatest_offset = _offset2nodeIndex.greatest_key();
    
    const auto node_sp = std::make_shared< Node >("New mesh", std::move(newMesh), greatest_offset);
    const auto nodeIndex = _nodesp.size();
    
    _offset2nodeIndex.assign(greatest_offset,
                             greatest_offset + node_sp->faceCount(),
                             nodeIndex);
    
    auto prev_vertex_count = 0;
    auto orig_vertex_count = 0;
    if ( !_nodesp.empty() ) {
        prev_vertex_count = startVertexOf(nodeIndex - 1) + indexCountOf(nodeIndex - 1   );
        orig_vertex_count = vertexStartOriginal(nodeIndex - 1) + indexCountOriginal(nodeIndex - 1);
    }
    _node2vertexStart[nodeIndex] = prev_vertex_count;
    _node2origVertexStart[nodeIndex] = orig_vertex_count;
    
    _nodesp.push_back(node_sp);
    
    pushBack( node_sp->triangulated_mesh() );
    
    return nodeIndex;
}

const uint64_t RawMeshData::nodeCount() const
{
    return _nodesp.size();
}
const std::string& RawMeshData::getNodeNameBy(uint64_t index) const
{
    return _nodesp[index]->name();
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
    
    auto nodeIdx = _offset2nodeIndex[faceId];
    
    if (nodeIdx != INVALID_NODE_INDEX)
    {
        _nodesp[nodeIdx]->triangulated_mesh()->changeColorFor(faceId, new_color);
        
        updateBuffers();
    }
    else {
        assert(false);
    }
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
    auto nodeIdx = _offset2nodeIndex[faceid];
    
    if (nodeIdx != INVALID_NODE_INDEX)
    {
        _nodesp[nodeIdx]->triangulated_mesh()->moveAlongNormal(faceid, offset);
        
        updateBuffers();
    }
    else {
        assert(false);
    }
}

uint32_t RawMeshData::originalVertexCount() const
{
    return static_cast<uint32_t>(_originalVertices.size());
}

void RawMeshData::setPosition(size_t nodeIndex, const vector_float3& newValue)
{
    _nodesp[nodeIndex]->setPosition(newValue);
}

vector_float3 RawMeshData::positionOf(size_t nodeIndex) const {
    return _nodesp[nodeIndex]->position();
}
matrix_float4x4 RawMeshData::transformOf(size_t nodeIndex) const {
    return _nodesp[nodeIndex]->transform();
}

uint32_t RawMeshData::indexCountOf(size_t nodeIndex) const {
    return 3 * _nodesp[nodeIndex]->faceCount();
}
uint64_t RawMeshData::indexCountOriginal(size_t nodeIndex) const {
    return _nodesp[nodeIndex]->vertexCount();
}
uint32_t RawMeshData::startVertexOf(size_t nodeIndex) const {
    auto it = _node2vertexStart.find(nodeIndex);
    if (it != _node2vertexStart.end()) {
        return it->second;
    }
    assert(false);
    return 0;
}
uint64_t RawMeshData::vertexStartOriginal(size_t nodeIndex) const {
    auto it = _node2origVertexStart.find(nodeIndex);
    if (it != _node2origVertexStart.end()) {
        return it->second;
    }
    assert(false);
    return INVALID_VERTEX_COUNT;
}

size_t RawMeshData::nodeIdBy(uint32_t faceId) const
{
    return _offset2nodeIndex[faceId];
}
