//
//  LowLevelWriter.hpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 03.12.2020.
//

#ifndef LowLevelWriter_hpp
#define LowLevelWriter_hpp

#include <stdio.h>
#include <fstream>
#include <string>


namespace Shapr3D {

class LowLevelWriter
{
public:
    LowLevelWriter(const std::string& file_path);
    ~LowLevelWriter();
    
    bool writeHeader(unsigned face_count);
    bool writeFace(const float* triangle_face, const float* normal);
    bool writeEnd();
    
private:
    
    std::ofstream _stream;
    
};

}
#endif /* LowLevelWriter_hpp */
