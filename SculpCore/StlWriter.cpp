//
//  StlWriter.cpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 03.12.2020.
//

#include "StlWriter.hpp"
#include "LowLevelWriter.hpp"
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_access.hpp> 

using namespace Shapr3D;

StlWriter::StlWriter(const std::string& file_path)
: _writer(new LowLevelWriter(file_path))
{
    
}

StlWriter::~StlWriter()
{
    delete _writer;
    _writer = nullptr;
}

void StlWriter::prepareGeometry(const IMesh& mesh, std::vector<NormalTrianglePair>& output_triangles) const
{
    output_triangles.clear();

    auto makeTrianglesFromFace = [this, &mesh, &output_triangles](auto& face) {
        auto n = mesh.getFaceNormal(face);
        std::array<float,3> normal = {A0(n), A1(n), A2(n)};

        size_t i = 0;
        size_t piv = i++;
        auto& piv_vert = face[piv];
        for (;i < face.size() - 1; ++i) {
            auto& fv = face[i];
            auto& fv_next = face[i+1];
            auto v = mesh.vertices()[fv.v];
            auto v_next = mesh.vertices()[fv_next.v];
            std::array<float, 9> triangle_face;
            
            triangle_face[0] = A0(mesh.vertices()[piv_vert.v]);
            triangle_face[1] = A1(mesh.vertices()[piv_vert.v]);
            triangle_face[2] = A2(mesh.vertices()[piv_vert.v]);
            
            triangle_face[3] = A0(v);
            triangle_face[4] = A1(v);
            triangle_face[5] = A2(v);
            
            triangle_face[6] = A0(v_next);
            triangle_face[7] = A1(v_next);
            triangle_face[8] = A2(v_next);
            
            output_triangles.push_back({normal, triangle_face});
        }
    };


    for (auto& p_face : mesh.faces()) {
        auto& face = *p_face;
        makeTrianglesFromFace(face);
    }
}

bool StlWriter::write(const IMesh& mesh)
{
    std::vector<NormalTrianglePair> triangles;
    
    prepareGeometry(mesh, triangles);

    auto face_count = static_cast<short>(triangles.size());
    _writer->writeHeader(face_count);

    for (auto& t : triangles)
    {
        _writer->writeFace(t.second.data(), t.first.data());
    }

    _writer->writeEnd();
    
    return false;
}

