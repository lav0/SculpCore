//
//  CommonFunctions.cpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 15.11.2020.
//

#include "CommonFunctions.hpp"
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>


using namespace Shapr3D;
using namespace boost::qvm;

bool Shapr3D::CommonFunctions::isFaceInsideOtherFace(const GeoTypes::Face& inner_face, const GeoTypes::Face& container_face)
{
    if (inner_face.size() > container_face.size()) {
        return false;
    }
    
    for (size_t i=0; i<container_face.size() - inner_face.size() + 1; ++i)
    {
        for (size_t j=0; j<inner_face.size(); ++j)
        {
            if (inner_face[j].v != container_face[i+j].v) {
                break;
            }
            
            if (j + 1 == inner_face.size()) {
                return true;
            }
        }
    }
    
    return false;
}

bool Shapr3D::CommonFunctions::operator==(const Shapr3D::GeoTypes::Face& a, const Shapr3D::GeoTypes::Face& b)
{
    assert(a.size() > 2);
    assert(b.size() > 2);
    
    if (a.size() != b.size())
        return false;
    
    auto a_vert_ind = 0;
    auto b_vert_ind = 0;
    bool b_vert_found = false;
    
    for (auto i=0; i<b.size(); ++i)
    {
        b_vert_ind = i;
        
        if (b[b_vert_ind].v == a[a_vert_ind].v) {
            b_vert_found = true;
            break;
        }
    }
    
    if (!b_vert_found)
        return false;
    
    auto ai = a_vert_ind;
    auto bi = b_vert_ind;
    
    do
    {
        ++ai;
        ++bi;
        
        if (ai == a.size()) {
            ai = 0;
        }
        if (bi == b.size()) {
            bi = 0;
        }
        
        if (a[ai].v != b[bi].v)
            return false;
        
    } while (ai != a_vert_ind || bi != b_vert_ind);
    
    return true;
}

float Shapr3D::CommonFunctions::getArea(const Shapr3D::GeoTypes::Face& face, const std::vector<Shapr3D::GeoTypes::Vec3>& vertices)
{
    assert(face.size() >= 2);
    
    auto area = 0.f;
    auto v0 = vertices[face[0].v];
    
    for (size_t i=2; i<face.size(); ++i) {
        auto v1 = vertices[face[i-1].v];
        auto v2 = vertices[face[i].v];
        
        area += 0.5f * boost::qvm::mag(boost::qvm::cross(v1 - v0, v2 - v1));
    }
    
    return area;
}

