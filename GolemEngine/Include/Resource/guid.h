#pragma once

#include <string>

#include "dll.h"


class GOLEM_ENGINE_API Guid
{
private:
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];

public:
    Guid();

    std::string ToString() const;
    void FromString(const std::string& _guidStr);
};
