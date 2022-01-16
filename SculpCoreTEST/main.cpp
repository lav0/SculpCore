//
//  main.cpp
//  SculpCoreTEST
//
//  Created by Andrey on 16.01.2022.
//

#include <iostream>
#include <cassert>
#include <iterator>
#include <boost/qvm/operations.hpp>
#include "../SculpCore/RawMeshData.hpp"
#include "../SculpCore/ObjReader.hpp"

using namespace Shapr3D;
void meshModelFaceColorChangeTest() {
    std::string file_path("/Users/Andrey/Documents/dev/Xcode-projects/sculpMe/Resources/cube-small.obj");
    auto reader = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    
    auto mesh = reader->load();
    
    auto faces = mesh->faces();
    auto verts = mesh->vertices();
    
    assert(faces.size() == 6);
    
    Vec4 default_face_color = {0.2f, 0.2f, 0.82, 1.f};
    Vec4 new_color = {0.5f, 0.7f, 0.5, 1.f};
    Vec4 new_color1 = {0.6f, 0.6f, 0.6, 1.f};
    
    for (auto& eachface : faces) {
        Vec4 eachcolor = mesh->getFaceColor(eachface);
        assert(boost::qvm::mag(eachcolor - default_face_color) < 1e-4);
    }
    
    mesh->changeColorFor(faces[0], new_color);
    mesh->changeColorFor(faces[3], new_color1);
    
    for (size_t i=0; i<faces.size(); ++i) {
        auto& eachface = faces[i];
        auto face_color = mesh->getFaceColor(eachface);
        if (i==0 || i==3) {
            assert(boost::qvm::mag(face_color - default_face_color) > 0.6);
        }
        else {
            assert(boost::qvm::mag(face_color - default_face_color) < 1e-4);
        }
    }
     
    printf("\n");
}

int main(int argc, const char * argv[]) {
    
    meshModelFaceColorChangeTest();
    
    std::string file_path("/Users/Andrey/Documents/dev/Xcode-projects/sculpMe/Resources/cube-small.obj");
    auto reader = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    
    auto raw_mesh_data = new RawMeshData(reader->load());
    
    auto indCount = raw_mesh_data->indicesCount();
    auto verCount = raw_mesh_data->vertexCount();
    const float* vertices = raw_mesh_data->lowLevelVertices();
    const uint32_t* faceids = raw_mesh_data->lowLevelFaceIds();
    simd_float4 v0 = {vertices[0], vertices[1], vertices[2], 1};
    
    assert(verCount == 36);
    assert(indCount == 36);

    auto a = simd_length(v0.xyz);
    auto b = sqrt(3*0.2*0.2);
    assert(fabs(a - b) < 1e-3);
    
    const simd_float4* colors = reinterpret_cast<const simd_float4*>(raw_mesh_data->lowLevelColors());
    
    for (auto i=0; i<indCount/3; i++) {
        printf("%u - (%f, %f, %f, %f)\n", faceids[3*i], colors[i].x, colors[i].y, colors[i].z, colors[i].w);
    }
    printf("\n");
    
    raw_mesh_data->changeColorForFace(1);
    raw_mesh_data->updateBuffers();
    const simd_float4* newColors = reinterpret_cast<const simd_float4*>(raw_mesh_data->lowLevelColors());
    
    for (auto i=0; i<indCount/3; i++) {
        printf("%u - (%f, %f, %f, %f)\n", faceids[3*i], newColors[i].x, newColors[i].y, newColors[i].z, newColors[i].w);
    }
    printf("\n");
    
    for (auto i=0; i<indCount; i+=3) {
        printf("%u - (%f, %f, %f)\n", faceids[i+2], vertices[i], vertices[i+1], vertices[i+2]);
    }
    
    delete raw_mesh_data;
    
    return 0;
}
