//
//  Triangulator.cpp
//  SculpCore
//
//  Created by Andrey on 29.03.2021.
//

#include "Triangulator.hpp"
#include "TrianMeshModel.hpp"


using namespace Shapr3D;

Triangulator::Triangulator(const IMesh& inmesh)
: _inmesh(inmesh)
{
    
}

std::unique_ptr<IMesh> Triangulator::triangulated_mesh() const
{
    std::vector<F> faces;
    triangulate(faces);
    
    return std::make_unique<TrianMeshModel<Shapr3D::DataPool>>(_inmesh.vertices(),
                                                               _inmesh.texture_vertices(),
                                                               _inmesh.normals(),
                                                               faces, 0
                                                               );
}

bool Triangulator::triangulate(std::vector<F>& faces) const
{
    return Triangulator::triangulate(_inmesh, faces);
}

bool Triangulator::triangulate(const IMesh& mesh, std::vector<Face>& out_faces)
{
    out_faces.clear();
    
    const auto& origin_faces = mesh.faces();
    
    bool mesh_has_changes = false;
    
    for (auto& p_origin_face : origin_faces)
    {
        assert(p_origin_face->size() >= 3);
        
        auto origin_face = *p_origin_face;
        
        auto pivot_vx = origin_face[0];
        auto secon_vx = origin_face[1];
        
        for (size_t i=2; i<origin_face.size(); ++i)
        {
            auto third_vx = origin_face[i];
            
            Face new_face(3);
            new_face[0] = pivot_vx;
            new_face[1] = secon_vx;
            new_face[2] = third_vx;
            
            out_faces.push_back(new_face);
            
            secon_vx = third_vx;
            
            if (i == 3)
                mesh_has_changes = true;
        }
        
    }
    
    return mesh_has_changes;
}
