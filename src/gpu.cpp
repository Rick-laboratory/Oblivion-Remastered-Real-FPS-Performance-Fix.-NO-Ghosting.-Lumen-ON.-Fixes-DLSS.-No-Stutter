#include "gpu.h"
#include <dxgi.h>
#include <memory>
#include <stdexcept>

namespace
{
struct ComRelease
{
    template <class T> void operator()(T *object) const noexcept
    {
        if (object)
            object->Release();
    }
};
} // namespace
std::vector<GpuInfo> enumerateGpus()
{
    IDXGIFactory *rawFactory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&rawFactory))))
        throw std::runtime_error("DXGI factory creation failed.");
    std::unique_ptr<IDXGIFactory, ComRelease> factory(rawFactory);
    std::vector<GpuInfo> result;
    for (unsigned index = 0;; ++index)
    {
        IDXGIAdapter *rawAdapter = nullptr;
        const auto status = factory->EnumAdapters(index, &rawAdapter);
        if (status == DXGI_ERROR_NOT_FOUND)
            break;
        if (FAILED(status))
            throw std::runtime_error("DXGI adapter enumeration failed.");
        std::unique_ptr<IDXGIAdapter, ComRelease> adapter(rawAdapter);
        DXGI_ADAPTER_DESC description{};
        if (FAILED(adapter->GetDesc(&description)))
            throw std::runtime_error("Cannot read DXGI adapter description.");
        result.push_back({index, description.Description, description.DedicatedVideoMemory});
    }
    return result;
}
