//
//  ObjReader.cpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 09.11.2020.
//

#include "ObjReader.hpp"
#include "MeshModel.hpp"
#include "TrianMeshModel.hpp" // tmp
#include "LowLevelReader.hpp"
#include <boost/algorithm/string.hpp>

using namespace Shapr3D;


template <typename V, typename F>
ObjReader<V, F>::ObjReader(std::string file_path)
: _low_reader(new LowLevelReader(file_path))
{
}

template <typename V, typename F>
ObjReader<V, F>::~ObjReader()
{
    delete _low_reader;
    _low_reader = nullptr;
}

template <typename VT, typename FT>
std::unique_ptr<IMesh> ObjReader<VT, FT>::load()
{
    std::vector<VT> vertices;
    std::vector<VT> vertex_textures;
    std::vector<VT> normals;
    std::vector<FT> faces;
    
    loadMesh(vertices, vertex_textures, normals, faces);
    
    return std::make_unique<MeshModel<Shapr3D::DataPool>>(vertices, vertex_textures, normals, faces);
}

template <typename V, typename F>
void ObjReader<V, F>::loadMesh(std::vector<V>& vertices,
                               std::vector<V>& vertex_textures,
                               std::vector<V>& normals,
                               std::vector<F>& faces)
{
    vertices.clear();
    vertex_textures.clear();
    normals.clear();
    faces.clear();
    
    auto addElement = [](std::vector<V>& container, std::vector<std::string>& raw_data) {
        if (raw_data.size() == VEC_DIMENSION) {
            container.push_back((V){
                std::stof(raw_data[0]),
                std::stof(raw_data[1]),
                std::stof(raw_data[2])
            });
            
            return true;
        }
        return false;
    };
    auto addUVElement = [](std::vector<V>& container, std::vector<std::string>& raw_data) {
        if (raw_data.size() == UV_DIMENSION) {
            container.push_back((V){
                std::stof(raw_data[0]),
                std::stof(raw_data[1])
            });
            
            return true;
        }
        return false;
    };
    
    std::vector<std::string> raw_data;
    bool read = false;
    while (!read) {
        auto res = _low_reader->readNextLine(raw_data);
        read = res == ReadingState::end;
        if (res == ReadingState::vertex)
        {
            if (!addElement(vertices, raw_data)) {
                throw std::runtime_error("Invalid obj format : expect /'v x y z/'");
            }
        }
        else if (res == ReadingState::texture_coord) {
            if (!addUVElement(vertex_textures, raw_data)) {
                throw std::runtime_error("Invalid obj format : expect /'vt u v w/'");
            }
        }
        else if (res == ReadingState::normal) {
            if (!addElement(normals, raw_data)) {
                throw std::runtime_error("Invalid obj format : expect /'vn i j k/'");
            }
        }
        else if (res == ReadingState::face) {
            F face;
            for (auto& entry : raw_data)
            {
                std::vector<std::string> fs;
                boost::split(fs, entry, boost::is_any_of("/"));
                if (fs.size() == VEC_DIMENSION) {
                    FaceVertex face_vert;
                    if (!fs[0].empty())
                        face_vert.v = std::stof(fs[0]) - 1;
                    if (!fs[1].empty())
                        face_vert.vt = std::stof(fs[1]) - 1;
                    if (!fs[2].empty())
                        face_vert.vn = std::stof(fs[2]) - 1;
                    
                    face.push_back(face_vert);
                }
                else {
                    throw std::runtime_error("Invalid obj format : expect 'f v/vt/vn '");
                }
            }
            
            faces.push_back(face);
        }
        raw_data.clear();
    }

}


namespace Shapr3D {
    template class ObjReader<Shapr3D::GeoTypes::Vec3, Shapr3D::GeoTypes::Face>;
}
