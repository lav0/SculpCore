//
//  Node.hpp
//  SculpCore
//
//  Created by Andrey on 25.01.2022.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <memory>
#include <string>
#include <simd/simd.h>
#include "TrianMeshModel.hpp"
#include "ObjReader.hpp"

namespace Shapr3D {
class Node
{
    using TrianMeshSp = std::shared_ptr<Shapr3D::TrianMeshModel<Shapr3D::DataPool>>;
    
public:
    
    Node(std::string name,
         std::string read_file_path,
         uint32_t    scene_faceid_offset);
    
    matrix_float4x4 transform() const;
    
    const TrianMeshSp& triangulated_mesh() const;
    
    const vector_float3& position() const {
        return _position;
    }
    void setPosition(const vector_float3& newValue) {
        _position = newValue;
    }
    
    size_t faceCount() const;
    
private:
    
    matrix_float4x4 positionMat() const;
    matrix_float4x4 scaleMat() const;
    matrix_float4x4 rotationMax() const;
    
private:
    
    std::string _name;
    
    TrianMeshSp _tmesh;
    std::unique_ptr<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>> _reader;
    
    vector_float3 _position = {0, 0, 0};
    vector_float3 _scale    = {1, 1, 1};
    vector_float3 _eulers   = {0, 0, 0};
    
};
}

#endif /* Node_hpp */
