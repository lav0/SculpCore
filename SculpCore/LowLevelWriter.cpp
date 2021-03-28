//
//  LowLevelWriter.cpp
//  Shapr3D_HomeTest
//
//  Created by Andrey on 03.12.2020.
//

#include "LowLevelWriter.hpp"


using namespace Shapr3D;

LowLevelWriter::LowLevelWriter(const std::string& file_path)
: _stream(file_path, std::ios::out | std::ios::binary)
{
    
}
LowLevelWriter::~LowLevelWriter()
{
    if (_stream.is_open()) {
        _stream.close();
    }
}

bool LowLevelWriter::writeHeader(unsigned face_count)
{
    if (_stream.is_open())
    {
        std::string header("Exported from Andrey's Home Assignment (sub 0)");

        _stream.write(header.c_str(), 80*sizeof(char));
        _stream.write(reinterpret_cast<const char*>(&face_count), sizeof(unsigned));

        return true;
    }
    
    return false;
}

bool LowLevelWriter::writeFace(const float* triangle_face, const float* normal)
{
    if (_stream.is_open())
    {
        _stream.write(reinterpret_cast<const char*>(normal), 3 * sizeof(float));
        _stream.write(reinterpret_cast<const char*>(triangle_face), 9 * sizeof(float));
        
        short face_end_mark = 0;
        _stream.write(reinterpret_cast<const char*>(&face_end_mark), 2);
        
        return true;
    }
    
    return false;
}

bool LowLevelWriter::writeEnd()
{
    return true;
}
