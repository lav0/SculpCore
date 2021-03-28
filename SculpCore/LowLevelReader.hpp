//
//  LowLevelReader.hpp
//  Shapr3D HomeTest
//
//  Created by Andrey on 07.11.2020.
//

#ifndef LowLevelReader_hpp
#define LowLevelReader_hpp

#include <string>
#include <fstream>
#include <map>
#include "IGeometryEntity.hpp"


namespace Shapr3D {

enum class ReadingState
{
    start,
    vertex,
    texture_coord,
    normal,
    face,
    end
};

class LowLevelReader
{
    std::map<std::string, ReadingState> OBJ_KEYS_MAP = {
        {"v", ReadingState::vertex},
        {"vt", ReadingState::texture_coord},
        {"vn", ReadingState::normal},
        {"f", ReadingState::face}
    };
    
public:
    
    LowLevelReader(std::string filePath);
    ~LowLevelReader();
    
    ReadingState readNextLine(std::vector<std::string>& data_output);
    
private:
    
    std::ifstream _stream;
    
};

}


#endif /* LowLevelReader_hpp */
