//
//  MeshModel.cpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 09.11.2020.
//

#include "MeshModel.hpp"
#include "CommonFunctions.hpp"
#include "Triangulator.hpp"
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>
#include <boost/qvm/swizzle.hpp>
#include <set>
#include <queue>


using namespace Shapr3D;
using namespace boost::qvm;


template <typename T>
MeshModel<T>::MeshModel(const std::vector<Vec3>& vertices,
                     const std::vector<Vec3>& texture_vertices,
                     const std::vector<Vec3>& normals,
                     const std::vector<Face>& faces)
: ParentDataPool(vertices, texture_vertices, normals)
{
    fillInHelperContainer(faces);
}

template <typename T>
void MeshModel<T>::fillInHelperContainer(const std::vector<Face>& faces)
{
    _mesh_faces.clear();
    _mesh_faces.reserve(faces.size());
    
    for (size_t i=0; i<faces.size(); ++i)
    {
        auto& face = faces[i];
        
        auto pface = std::make_shared<Face>(face);
        
        _mesh_faces.push_back(pface);
        _face_colors.emplace(pface, GeoTypes::DEFAULT_FACE_COLOR);
        
        for (auto& fv : face)
        {
            ParentDataPool::_indices.push_back(fv.v);
        }
    }
    
    _vertex_colors.reserve(ParentDataPool::_vertices.size());
    for (size_t vertex_index=0; vertex_index < ParentDataPool::_vertices.size(); ++vertex_index)
    {
        _vertex_colors.push_back(DEFAULT_VERT_COLOR);
    }
}

template <typename T>
float MeshModel<T>::surfaceArea() const
{
    auto surface_area = 0.f;
    
    for (auto& p_face : _mesh_faces)
    {
        surface_area += CommonFunctions::getArea(*p_face, vertices());
    }
    
    return surface_area;
}

template <typename T>
float MeshModel<T>::volume() const
{
    auto volume = 0.f;
    
    auto centre = Vec3({0,0,0});
        
    for (auto& p_face : _mesh_faces)
    {
        auto normal = getFaceNormal(*p_face);
        
        auto point_along_normal = ParentDataPool::_vertices[(*p_face)[0].v] + normal;
        
        auto off_face_dist = getDistanceToFaceFrom(point_along_normal, *p_face);
        
        auto face_area = CommonFunctions::getArea(*p_face, vertices());
        auto height = getDistanceToFaceFrom(centre, *p_face);
        
        volume -= face_area * (height*off_face_dist) / 3.f;
    }
    
    return volume;
}

template <typename T>
float MeshModel<T>::getDistanceToFaceFrom(const Vec3& point, const Face& face) const
{
    auto normal = getFaceNormal(face);
    
    auto face_point = ParentDataPool::_vertices[face[0].v];
    auto D = - dot(face_point, normal);
    auto sign = 1.f;
    if (abs(D) > 1e-5) {
        sign = D / abs(D);
    }
    
    auto nue = (-sign) / sqrt(dot(normal, normal));
    
    return nue * (dot(point, normal) + D);
}

template <typename T>
GeoTypes::Vec3 MeshModel<T>::findIntersection(const Shapr3D::Line3D& line, const GeoTypes::Face& face) const
{
    auto vector = line.vector;
    auto point = line.point;
    
    auto normal = getFaceNormal(face);
    
    auto face_point = ParentDataPool::_vertices[face[0].v];
    auto D = - dot(face_point, normal);
    
    auto a = A0(normal);
    auto b = A1(normal);
    auto c = A2(normal);
    auto d = A0(point);
    auto e = A1(point);
    auto f = A2(point);
    auto k = A0(vector);
    auto m = A1(vector);
    auto r = A2(vector);
    
    float x=0, y=0, z=0;
    if (r*r > 1e-6) {
        auto CC = c*r + a*k + b*m;
        z = (a*k*f - a*d*r + b*m*f - b*e*r - D*r) / CC;
        x = (k*z + r*d - k*f) / r;
        y = (m*z + r*e - m*f) / r;
    }
    else
    if (k*k > 1e-6) {
        auto AA = (a*k + m*b + c*r);
        x = (m*b*d - b*e*k + c*r*d - c*f*k - D*k) / AA;
        y = (m*x - m*d + k*e) / k;
        z = (r*x - r*d + k*f) / k;
    }
    else if (m*m > 1e-6) {
        auto BB = b*m + k*a + c*r;
        y = (c*r*e - c*f*m - a*d*m + a*k*e - D*m) / BB;
        x = (k*y + m*d - k*e) / m;
        z = (r*y - r*e + m*f) / m;
    }
    
    return {x,y,z};
}

template <typename T>
GeoTypes::Vec3 MeshModel<T>::projectPointOnFacePlane(const GeoTypes::Vec3& point, const GeoTypes::Face& face) const
{
    auto normal = getFaceNormal(face);
    
    return findIntersection({point, normal}, face);
}

template <typename T>
bool MeshModel<T>::isPointInsideFace(const GeoTypes::Vec3& point, const GeoTypes::Face& face) const
{
    auto normal = getFaceNormal(face);
    
    auto fp0 = ParentDataPool::_vertices[face.back().v];
    
    if (mag(fp0 - point) < 1e-6) {
        return true;
    }
    
    for (size_t i=0; i<face.size(); ++i)
    {
        auto& fp1 = ParentDataPool::_vertices[face[i].v];
        
        if (mag(fp1 - point) < 1e-6) {
            return true;
        }
        
        auto v0 = fp1 - fp0;
        auto v1 = point - fp1;
        
        auto c = boost::qvm::cross(v0, v1);
        
        if (dot(normal, c) < -1e-5) {
            return false;
        }
        
        fp0 = fp1;
    }
    
    return true;
}

template <typename T>
bool MeshModel<T>::isPointInside(const Vec3& point) const
{
    auto getFirstFace = [this, &point]() {
        for (auto& p_face : _mesh_faces) {
            auto norm_along = faceGravityCentre(*p_face) - point;
            try {
                normalize(norm_along);
            }
            catch (zero_magnitude_error& e) { continue; }
            auto d = dot(getFaceNormal(*p_face), norm_along);
            if (d*d > 0.1) {
                return p_face;
            }
        }
        return _mesh_faces.front();
    };
    
    auto first = getFirstFace();
    auto gc = faceGravityCentre(*first);
    auto along_vec = gc - point;
    
    struct Vec3Cmp
    {
        bool operator()(const Vec3& a, const Vec3& b) const {
            auto eps = 1e-8;
            auto x = (A0(a) - A0(b))*(A0(a) - A0(b)) > eps;
            auto y = (A1(a) - A1(b))*(A1(a) - A1(b)) > eps;
            auto z = (A2(a) - A2(b))*(A2(a) - A2(b)) > eps;
            return x || y || z;
        }
    };
    std::set<Vec3, Vec3Cmp> intersections = {gc};
    
    for (auto& p_face : _mesh_faces)
    {
        if (p_face.get() == first.get())
            continue;
        
        auto face_normal = getFaceNormal(*p_face);
        
        auto dot_prod = dot(face_normal, along_vec); // norm
        
        if (dot_prod * dot_prod < 1e-6)
            continue;
        
        auto intex = findIntersection({point, along_vec}, *p_face);
        
        if (isPointInsideFace(intex, *p_face)) {
            intersections.insert(intex);
        }
    }
    
    auto inx_counter = 0;
    
    for (auto& inx_point : intersections)
    {
        auto vector = inx_point - point;
        if (mag(vector) < 1e-5 || dot(vector, along_vec) > 0) {
            ++inx_counter;
        }
    }
    
    return (inx_counter % 2) == 1;
}

template <typename T>
GeoTypes::Vec3 MeshModel<T>::faceGravityCentre(const GeoTypes::Face& face) const
{
    Vec3 gc = {0,0,0};
    
    for (auto& fv : face)
    {
        gc += ParentDataPool::_vertices[fv.v];
    }
    
    return gc / (1.f * face.size());
}

template <typename T>
GeoTypes::Vec3 MeshModel<T>::getFaceNormal(const Face& face, size_t last_index) const
{
   auto N = ParentDataPool::_vertices[face[0].v];
   auto M = ParentDataPool::_vertices[face[1].v];
   auto K = ParentDataPool::_vertices[face[last_index].v];
   
   return boost::qvm::cross(M - N, K - M);
}

template <typename T>
GeoTypes::Vec3 MeshModel<T>::getFaceNormal(const Face& face) const
{
    assert(face.size() >= 3);
 
    Vec3 normal = {0,0,0};
    for (size_t last_index=2; last_index<face.size(); ++last_index) {
        normal = getFaceNormal(face, 2);
        
        try {
            boost::qvm::normalize(normal);
        }
        catch (zero_magnitude_error& error) {
            if (last_index == face.size()-1) {
                throw error;
            }
            continue;
        }
    }
    
    return normal;
}

template<typename T>
bool MeshModel<T>::transform(const boost::qvm::mat<float,4,4> trs)
{
    for (auto& vertex : ParentDataPool::_vertices)
    {
        boost::qvm::vec<float, 4> v = XYZ1(vertex);
        
        vertex = XYZ(v);
//        vertex = XYZ(trs * v);
    }
    
    for (auto& normal : ParentDataPool::_normals)
    {
        boost::qvm::vec<float, 4> n = XYZ0(normal);
        
        normal = XYZ(trs * n);
    }
    
    return false;
}

template<typename T>
const Vec4& MeshModel<T>::getFaceColor(const std::shared_ptr<GeoTypes::Face>& face) const
{
    static Vec4 v = {0, 0, 0, 1};
    
    auto face_color_pair = _face_colors.find(face);
    if (face_color_pair != _face_colors.end()) {
        return face_color_pair->second;
    }
    
    return v;
}

template<typename T>
const Vec4& MeshModel<T>::getVertexColor(size_t vertex_index) const
{
    assert(vertex_index < _vertex_colors.size());
    return _vertex_colors[vertex_index];
}

template<typename T>
bool MeshModel<T>::changeColorFor(const uint32_t& faceid, const Vec4& new_color)
{
    auto face = faceBy(faceid);
    auto face_color_pair = _face_colors.find(face);
    if (face_color_pair != _face_colors.end()) {
        face_color_pair->second = new_color;
        return true;
    }
    
    return false;
}

template<typename T>
bool MeshModel<T>::moveAlongNormal(const uint32_t& faceid, const float offset)
{
    auto face = faceBy(faceid);
    
    if (nullptr == face) {
        return false;
    }
    
    for (auto& facevert : *face) {
        // TODO: tmp : make sure all face's vertices have same normals
    }
    
    if (offset <= 0) {
        return false; // tmp
    }
    
    auto normal = ParentDataPool::_normals[face->front().vn];
    
    for (auto& facevert : *face) {
        ParentDataPool::_vertices[facevert.v] += normal * offset;
    }
    
    return true;
}

template<typename T>
bool MeshModel<T>::getFaceId(const std::shared_ptr<GeoTypes::Face>& face, uint32_t& faceid_out) const
{
    auto itr = std::find(_mesh_faces.begin(), _mesh_faces.end(), face);
    
    if (itr != _mesh_faces.end()) {
        faceid_out = itr - _mesh_faces.begin();
        return true;
    }
    
    return false;
}

template<typename T>
std::shared_ptr<Face> MeshModel<T>::faceBy(uint32_t faceid) const
{
    if (faceid < 0 || faceid >= _mesh_faces.size()) {
        return nullptr;
    }
    
    return _mesh_faces[faceid];
}

namespace Shapr3D {
    template class MeshModel<DataPool>;
}

