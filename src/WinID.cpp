
#include "WinID.hpp"

namespace
{
uint32_t wid = 0;
}

namespace WinID
{

void setWinID(uint32_t id)
{
    wid = id;
}

uint32_t getWinID()
{
    return wid;
}

}
