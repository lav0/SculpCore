//
//  MeshBuilder.cpp
//  SculpCore
//
//  Created by Andrey on 26.01.2022.
//

#include "MeshBuilder.hpp"
#include "MeshModel.hpp"
#include <memory>

namespace Shapr3D {

class MeshBuilder
{
public:
    static std::unique_ptr<IMesh> buildTriangle()
    {
        std::vector<Vec3> vertices = {
            {0, 0, 0},
            {0.2f, 0, 0},
            {0, 0.2f, 0}
        };
        std::vector<Vec3> texture_vertices;
        std::vector<Vec3> normals = {
            {0, 0, 1}
        };
        
        FaceVertex fv0, fv1, fv2;
        fv0.v = 0; fv0.vn = 0;
        fv1.v = 2; fv1.vn = 0;
        fv2.v = 1; fv2.vn = 0;
        
        std::vector<Face> faces = { Face{fv0, fv1, fv2} };
        
        return std::make_unique< MeshModel<DataPool> >(vertices, texture_vertices, normals, faces);
    }
    
    static std::unique_ptr<IMesh> buildSquare()
    {
        std::vector<Vec3> vertices = {
            {0, 0, 0},
            {0, 0, 1},
            {1, 0, 1},
            {1, 0, 0}
        };
        std::vector<Vec3> texture_vertices;
        std::vector<Vec3> normals = {
            {0, 1, 0}
        };
        
        FaceVertex f0, f1, f2, f3;
        f0.v = 0; f0.vn = 0;
        f1.v = 1; f1.vn = 0;
        f2.v = 2; f2.vn = 0;
        f3.v = 3; f3.vn = 0;
        
        std::vector<Face> faces = { Face{f0, f1, f2, f3} };
        
        return std::make_unique< MeshModel<DataPool> >(vertices, texture_vertices, normals, faces);
    }
    
};
}
