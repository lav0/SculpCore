//
//  LowLevelReader.cpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 07.11.2020.
//

#include "LowLevelReader.hpp"
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include <deque>


using namespace Shapr3D;

LowLevelReader::LowLevelReader(std::string filePath)
: _stream(filePath, std::ios::in)
{
}
LowLevelReader::~LowLevelReader()
{
    if (_stream.is_open()) {
        _stream.close();
    }
}
ReadingState LowLevelReader::readNextLine(std::vector<std::string>& data_output)
{
    if (_stream.is_open())
    {
        std::string line;
        
        while (std::getline(_stream, line))
        {
            std::deque<std::string> strs;
            boost::split(strs,line,boost::is_any_of(" "));
            
            if (strs.empty()) {
                
            }
            else {
                auto key = strs.front();
                strs.pop_front();
                
                if (OBJ_KEYS_MAP.find(key) != OBJ_KEYS_MAP.end())
                {
                    data_output.clear();
                    data_output.insert(data_output.begin(), strs.begin(), strs.end());
                    
                    return OBJ_KEYS_MAP[key];
                }
            }
            
            line.clear();
        }
    }
    else
    {
        throw std::runtime_error("File not found");
    }
    
    return (!_stream.eof()) ? ReadingState::start : ReadingState::end;
}
