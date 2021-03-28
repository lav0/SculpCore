//
//  SculpCore.cpp
//  SculpCore
//
//  Created by Andrey on 28.03.2021.
//

#include <iostream>
#include "SculpCore.hpp"
#include "SculpCorePriv.hpp"

void SculpCore::HelloWorld(const char * s)
{
    SculpCorePriv *theObj = new SculpCorePriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void SculpCorePriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

