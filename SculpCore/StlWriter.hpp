//
//  StlWriter.hpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 03.12.2020.
//

#ifndef StlWriter_hpp
#define StlWriter_hpp

#include <stdio.h>

#include <array>
#include "MeshModel.hpp"

#pragma GCC visibility push(default)


namespace Shapr3D {

class LowLevelWriter;

class StlWriter
{
    using NormalTrianglePair = std::pair<std::array<float, 3>, std::array<float, 9>>;
    
public:
    StlWriter(const std::string& file_path);
    ~StlWriter();
    
    bool write(const IMesh& mesh);
    
private:
    
    void prepareGeometry(const IMesh& mesh, std::vector<NormalTrianglePair>& output_triangles) const;
    
private:
    
    LowLevelWriter* _writer;
};

}

#pragma GCC visibility pop
#endif /* StlWriter_hpp */
