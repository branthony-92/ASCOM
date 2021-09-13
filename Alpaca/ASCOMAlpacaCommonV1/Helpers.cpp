#include "stdafx.h"
#include "Helpers.h"

int Alpaca::extractDeviceNum(std::string target)
{
    try
    {
        // template: /api/v1/{device type}/{device num}/{endpoint}
        auto offsetL = 0;

        for (auto i = 0; i < 3; i++)
        {
            // find the '/' just before {device num}
            offsetL = target.find_first_of('/', offsetL + 1);
        }

        // now find the next '/' just after the {device num}
        auto offsetR = target.find_first_of('/', offsetL + 1);
        auto len = offsetR - offsetL - 1;
        int val = std::stoi(target.substr(offsetL + 1, len));

        return val;
    }
    catch (std::exception)
    {
        return -1;
    }
}