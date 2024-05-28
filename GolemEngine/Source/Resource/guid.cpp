#include "Resource/guid.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <ctime>


Guid::Guid()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dist(0, ULONG_MAX);

    data1 = dist(gen);
    data2 = dist(gen) & 0xFFFF;
    data3 = dist(gen) & 0xFFFF;
    for (int i = 0; i < 8; i++)
    {
        data4[i] = (dist(gen) >> (i * 8)) & 0xFF;
    }
}

std::string Guid::ToString() const
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << std::setw(8) << data1 << '-'
        << std::setw(4) << data2 << '-'
        << std::setw(4) << data3 << '-';

    for (int i = 0; i < 2; ++i)
    {
        oss << std::setw(2) << static_cast<unsigned>(data4[i]);
    }
    oss << '-';

    for (int i = 2; i < 8; ++i)
    {
        oss << std::setw(2) << static_cast<unsigned>(data4[i]);
    }
    return oss.str();
}

void Guid::FromString(const std::string& _guidStr)
{
    std::istringstream iss(_guidStr);

    char dummy;
    iss >> std::hex >> data1 >> dummy
        >> std::hex >> data2 >> dummy
        >> std::hex >> data3 >> dummy;

    for (int i = 0; i < 2; ++i) {
        int temp;
        iss >> std::hex >> temp;
        data4[i] = static_cast<unsigned char>(temp);
    }

    iss >> dummy;

    for (int i = 2; i < 8; ++i) {
        int temp;
        iss >> std::hex >> temp;
        data4[i] = static_cast<unsigned char>(temp);
    }
}