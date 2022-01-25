//
//  Node.cpp
//  SculpCore
//
//  Created by Andrey on 25.01.2022.
//

#include "Node.hpp"

using namespace Shapr3D;

Node::Node(std::string name,
           std::string read_file_path,
           uint32_t    scene_faceid_offset)
: _name(name)
, _reader(std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(read_file_path))
{
    auto mesh = _reader->load();
    
    std::shared_ptr<IMesh> omesh = std::move(mesh);
    _tmesh = std::make_shared< TrianMeshModel<DataPool> >(omesh, 1);
}

matrix_float4x4 Node::transform() const
{
    auto pos = positionMat();
    auto rot = rotationMax();
    auto scl = scaleMat();
    return simd_mul(pos, simd_mul(rot, scl));
}

const Node::TrianMeshSp& Node::triangulated_mesh() const
{
    return _tmesh;
}

matrix_float4x4 Node::positionMat() const
{
    auto [x, y, z] = std::make_tuple(_position.x, _position.y, _position.z);
    
    auto mat = matrix_identity_float4x4;
    
    mat.columns[3][0] = x;
    mat.columns[3][1] = y;
    mat.columns[3][2] = z;

    return mat;
}
matrix_float4x4 Node::scaleMat() const
{
    // TODO: impl
    return matrix_identity_float4x4;
}
matrix_float4x4 Node::rotationMax() const
{
    // TODO: impl
    return matrix_identity_float4x4;
}

size_t Node::faceCount() const
{
    return _tmesh->faces().size();
}
