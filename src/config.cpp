#include "config.h"
#include "config_template.h"
#include <windows.h>

#include <atomic>
#include <limits>
#include <stdexcept>
#include <system_error>

namespace
{
[[noreturn]] void fail(const char *operation)
{
    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), operation);
}
std::filesystem::path uniqueSibling(const std::filesystem::path &target, const wchar_t *suffix)
{
    static std::atomic<unsigned> sequence{0};
    auto result = target;
    result += L"." + std::to_wstring(GetCurrentProcessId()) + L"." + std::to_wstring(GetTickCount64()) +
              L"." + std::to_wstring(sequence++) + suffix;
    return result;
}
} // namespace

unsigned parseUnsigned(std::wstring_view value, unsigned minimum, unsigned maximum)
{
    if (value.empty())
        throw std::invalid_argument("Expected an unsigned decimal number.");
    unsigned result = 0;
    for (const auto ch : value)
    {
        if (ch < L'0' || ch > L'9' || result > maximum / 10 ||
            (result == maximum / 10 && static_cast<unsigned>(ch - L'0') > maximum % 10))
            throw std::invalid_argument("Number is invalid or outside the supported range.");
        result = result * 10 + static_cast<unsigned>(ch - L'0');
    }
    if (result < minimum)
        throw std::invalid_argument("Number is below the supported range.");
    return result;
}

unsigned poolFromDedicatedBytes(std::uint64_t bytes)
{
    // Retains the original 95% heuristic; dedicated capacity is not a live memory budget.
    const auto pool = (bytes / (1024 * 1024)) * 95 / 100;
    if (pool < 1 || pool > 1048576)
        throw std::runtime_error("No usable dedicated VRAM value. Supply --pool-mib explicitly.");
    return static_cast<unsigned>(pool);
}

std::string renderConfig(unsigned poolMiB)
{
    if (poolMiB < 1 || poolMiB > 1048576)
        throw std::invalid_argument("Pool size must be between 1 and 1048576 MiB.");
    std::string result(enginePreset);
    const std::string marker = "{POOLSIZE}";
    const auto replacement = std::to_string(poolMiB);
    std::size_t offset = 0;
    unsigned replacements = 0;
    while ((offset = result.find(marker, offset)) != std::string::npos)
    {
        result.replace(offset, marker.size(), replacement);
        offset += replacement.size();
        ++replacements;
    }
    if (replacements == 0)
        throw std::runtime_error("Preset has no pool-size marker.");
    return result;
}

std::filesystem::path writeConfig(const std::filesystem::path &target, std::string_view contents)
{
    if (contents.empty() || contents.size() > std::numeric_limits<DWORD>::max())
        throw std::invalid_argument("Invalid configuration size.");
    if (target.filename().empty() ||
        !std::filesystem::is_directory(target.parent_path().empty() ? std::filesystem::path(L".")
                                                                    : target.parent_path()))
        throw std::invalid_argument("Choose a file inside an existing directory.");
    const bool exists = std::filesystem::exists(target);
    if (exists && !std::filesystem::is_regular_file(target))
        throw std::invalid_argument("Target is not a regular file.");
    if (exists && (GetFileAttributesW(target.c_str()) & FILE_ATTRIBUTE_READONLY))
        throw std::invalid_argument("Target is read-only.");
    const auto temporary = uniqueSibling(target, L".tmp");
    HANDLE file =
        CreateFileW(temporary.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        fail("Create temporary configuration");
    try
    {
        DWORD written = 0;
        if (!WriteFile(file, contents.data(), static_cast<DWORD>(contents.size()), &written, nullptr))
            fail("Write configuration");
        if (written != contents.size())
            throw std::runtime_error("Incomplete configuration write.");
        if (!FlushFileBuffers(file))
            fail("Flush configuration");
        const BOOL closed = CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
        if (!closed)
            fail("Close configuration");

        std::filesystem::path backup;
        if (exists)
        {
            backup = uniqueSibling(target, L".bak");
            if (!CopyFileW(target.c_str(), backup.c_str(), TRUE))
                fail("Back up existing configuration");
        }
        // The complete sibling file replaces the destination only after a successful backup.
        const DWORD flags = MOVEFILE_WRITE_THROUGH | (exists ? MOVEFILE_REPLACE_EXISTING : 0);
        if (!MoveFileExW(temporary.c_str(), target.c_str(), flags))
            fail("Replace configuration");
        return backup;
    }
    catch (...)
    {
        if (file != INVALID_HANDLE_VALUE)
            CloseHandle(file);
        DeleteFileW(temporary.c_str());
        throw;
    }
}
