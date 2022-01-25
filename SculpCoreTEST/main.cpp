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
#include "../SculpCore/MeshModel.hpp"
#include "../SculpCore/TrianMeshModel.hpp"
#include "../SculpCore/RawMeshData.hpp"
#include "../SculpCore/ObjReader.hpp"

using namespace Shapr3D;
bool meshModelFaceColorChangeTest(std::unique_ptr<IMesh>&& mesh) {
    
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
    
    bool done = true;
    uint32_t face0_id; mesh->getFaceId(faces[0], face0_id);
    uint32_t face3_id; mesh->getFaceId(faces[3], face3_id);
    done &= mesh->changeColorFor(face0_id, new_color);
    done &= mesh->changeColorFor(face3_id, new_color1);
    
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
    
    assert(done);
    printf("Face Color Chnage : success\n");
    
    return true;
}

bool meshModelFaceMoveAlongNormalTest(std::unique_ptr<IMesh>&& mesh) {
    const auto& faces = mesh->faces();
    const auto& verts = mesh->vertices();
    const auto& norms = mesh->normals();
    
    assert(faces.size() == 6);
    assert(verts.size() == 8);
    assert(norms.size() == 6);

    
    for (size_t i=0; i<faces.size(); ++i) {
        auto& eachface = faces[i];
        auto& normal = norms[i];
        for (auto& facevert : *eachface) {
            auto& vertex = verts[facevert.v];
            
            // as we dealing with a cube we get an offset from zero by along the normal
            auto side_offset = boost::qvm::dot(normal, vertex);
            assert(fabs(side_offset - 0.2f) < 1e-4);
        }
    }
    
    float offset = 0.7f;
    uint32_t faceid1; mesh->getFaceId(faces[1], faceid1);
    bool done = mesh->moveAlongNormal(faceid1, offset);
    
    auto new_verts = mesh->vertices();
    
    for (size_t i=0; i<faces.size(); ++i) {
        auto& eachface = faces[i];
        auto& normal = norms[i];
        for (auto& facevert : *eachface) {
            auto& vertex = new_verts[facevert.v];
            
            // as we dealing with a cube we get an offset from zero by along the normal
            auto side_offset = boost::qvm::dot(normal, vertex);
            if (i==1) {
                assert(fabs(side_offset - 0.9f) < 1e-4);
            }
            else {
                assert(fabs(side_offset - 0.2f) < 1e-4);
            }
        }
    }
    
    assert(done);
    printf("Face Move Along normal : success\n");
    
    return true;
}

bool meshModelTriangulateTest() {
    
    FaceVertex fv0; fv0.v = 0; fv0.vn = 1;
    FaceVertex fv1; fv1.v = 0; fv1.vn = 1;
    FaceVertex fv2; fv2.v = 0; fv2.vn = 1;
    FaceVertex fv3; fv3.v = 0; fv3.vn = 1;
    auto face = Face{fv0, fv1, fv2, fv3};
    
    const std::vector<Vec3> vertices = {Vec3{0, 0, 0}, Vec3{0, 1, 0}, Vec3{1, 1, 0}, Vec3{1, 0, 0}};
    const std::vector<Vec3> texture_vertices = {};
    const std::vector<Vec3> normals = {Vec3{0, 0, 1}};
    const std::vector<Face> faces = { face };
    
    auto mesh = TrianMeshModel<DataPool>(vertices, texture_vertices, normals, faces, 1);
    
    auto tfaces = mesh.faces();
    
    assert(tfaces.size() == 2);
    
    uint32_t faceid0 = 0, faceid1 = 0, faceid = 0;
    bool validId = true;
    validId &= mesh.getFaceId(tfaces[0], faceid0);
    validId &= mesh.getFaceId(tfaces[1], faceid1);
    validId &= mesh.getFaceId(mesh.faces()[0], faceid);
    
    assert(validId);
    assert(faceid == faceid0);
    assert(faceid == faceid1);
    printf("Triangulate and Face IDs : success\n");
    
    return true;
}

bool trianMeshModelByIMesh(std::unique_ptr<IMesh>&& mesh)
{
    std::shared_ptr<IMesh> omesh = std::move(mesh);
    auto tmesh = std::make_unique< TrianMeshModel<DataPool> >(omesh, 1);
    
    auto& overtices = omesh->vertices();
    auto ofaces = omesh->faces();
    auto& tvertices = tmesh->vertices();
    auto tfaces = tmesh->faces();
    
    assert(ofaces.size() == 6);
    assert(tfaces.size() == 12);
    
    assert(overtices.size() == 8);
    assert(tvertices.size() == 8);
    
    auto test_vertices = std::vector<Vec3>{
        Vec3({0.200000, 0.200000, -0.200000}),
        Vec3({0.200000, -0.200000, -0.200000}),
        Vec3({-0.200000, -0.200000, -0.200000}),
        Vec3({-0.200000, 0.200000, -0.200000}),
        Vec3({0.200000, 0.200000, 0.200000}),
        Vec3({0.200000, -0.200000, 0.200000}),
        Vec3({-0.200000, -0.200000, 0.200000}),
        Vec3({-0.200000, 0.200000, 0.200000})
    };
    
    for (size_t i=0; i<overtices.size(); ++i) {
        bool oeq = boost::qvm::mag(overtices[i] - test_vertices[i]) < 1e-4;
        bool teq = boost::qvm::mag(tvertices[i] - test_vertices[i]) < 1e-4;
        assert(oeq);
        assert(teq);
    }
    
    // check if triangulated faces point to the same vertices as origin mesh
    for (auto& tface : tfaces)
    {
        for (auto& face_vertex : *(tface)) {
            auto vidx = face_vertex.v;
            auto ov = overtices[vidx];
            auto tv = tvertices[vidx];
                
            bool eq = boost::qvm::mag(ov - tv) < 1e-4;
            assert(eq);
        }
    }
    // now check if origin faces point to the triangulated mesh vertices
    for (auto& oface : ofaces)
    {
        for (auto& face_vertex : *(oface)) {
            auto vidx = face_vertex.v;
            auto ov = overtices[vidx];
            auto tv = tvertices[vidx];
            
            bool eq = boost::qvm::mag(ov - tv) < 1e-4;
            assert(eq);
        }
    }
    
    tmesh->moveAlongNormal(3, 1.f);
    
    std::set<size_t> exceptions = {0, 1, 4, 5};
    for (size_t i=0; i<overtices.size(); ++i) {
        bool oeq = boost::qvm::mag(overtices[i] - test_vertices[i]) < 1e-4;
        bool teq = boost::qvm::mag(tvertices[i] - test_vertices[i]) < 1e-4;
        assert(oeq == teq);
        if (exceptions.count(i) == 0) {
            assert(oeq);
        }
        else {
            assert(!oeq);
        }
    }
    
    omesh.reset();
    
    auto pface0 = tmesh->faces()[0];
    auto pface1 = tmesh->faces()[1];
    
    uint32_t faceid0=0, faceid1=0;
    bool valid0 = tmesh->getFaceId(pface0, faceid0);
    bool valid1 = tmesh->getFaceId(pface1, faceid1);
    
    assert(valid0);
    assert(valid1);
    assert(faceid1 == faceid0);
    assert(faceid1 == 1);
    
    printf("TrianMeshModel by IMesh : success\n");
    return true;
}

int main(int argc, const char * argv[]) {
    std::string file_path("/Users/Andrey/Documents/dev/Xcode-projects/sculpMe/Resources/cube-small.obj");
    auto reader1 = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    auto reader2 = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    auto reader3 = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    auto reader4 = std::make_unique<Shapr3D::ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>>(file_path);
    
    auto raw_mesh_data = new RawMeshData(reader1->load());
    
    meshModelFaceColorChangeTest(reader2->load());
    meshModelFaceMoveAlongNormalTest(reader3->load());
    meshModelTriangulateTest();
    trianMeshModelByIMesh(reader4->load());
    
    auto indCount = raw_mesh_data->indicesCount();
    auto verCount = raw_mesh_data->vertexCount();
    const float* vertices = raw_mesh_data->lowLevelVertices();
    const uint32_t* faceids = raw_mesh_data->lowLevelFaceIds();
    simd_float4 v0 = {vertices[0], vertices[1], vertices[2], 1};
    
    assert(verCount == 24);
    assert(indCount == 24);

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
