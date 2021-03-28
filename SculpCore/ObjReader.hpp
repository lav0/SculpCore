//
//  ObjReader.hpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 09.11.2020.
//

#ifndef ObjReader_hpp
#define ObjReader_hpp

#include "CommonTypes.h"
#include "IMesh.h"
#include <memory>

#pragma GCC visibility push(default)


namespace Shapr3D {

class LowLevelReader;

template <typename V, typename F>
class ObjReader
{
    constexpr static size_t VEC_DIMENSION = 3;
    constexpr static size_t UV_DIMENSION = 2;
public:
    
    ObjReader(std::string file_path);
    ~ObjReader();
    
    std::unique_ptr<IMesh> load();
    
private:
    
    void loadMesh(std::vector<V>& vertices,
                  std::vector<V>& vertex_textures,
                  std::vector<V>& normals,
                  std::vector<F>& faces);
    
private:
    
    LowLevelReader* _low_reader;
};

}

#pragma GCC visibility pop
#endif /* ObjReader_hpp */
