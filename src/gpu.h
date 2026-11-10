#pragma once
#include <cstdint>
#include <string>
#include <vector>
struct GpuInfo
{
    unsigned index = 0;
    std::wstring name;
    std::uint64_t dedicatedBytes = 0;
};
std::vector<GpuInfo> enumerateGpus();
