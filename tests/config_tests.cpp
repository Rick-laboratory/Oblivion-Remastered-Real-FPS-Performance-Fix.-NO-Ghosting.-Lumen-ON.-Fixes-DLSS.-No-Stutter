#include "config.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
namespace fs = std::filesystem;
void require(bool ok, const char *message)
{
    if (!ok)
        throw std::runtime_error(message);
}
template <class F> void rejects(F action)
{
    bool rejected = false;
    try
    {
        action();
    }
    catch (const std::exception &)
    {
        rejected = true;
    }
    require(rejected, "Expected rejection.");
}
std::string read(const fs::path &path)
{
    std::ifstream input(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(input), {}};
}
int main()
{
    const auto root =
        fs::temp_directory_path() / (L"oblivion-config-test-" + std::to_wstring(GetCurrentProcessId()) +
                                     L"-" + std::to_wstring(GetTickCount64()));
    try
    {
        require(poolFromDedicatedBytes(8ULL * 1024 * 1024 * 1024) == 7782, "Pool calculation");
        rejects([] { poolFromDedicatedBytes(0); });
        require(parseUnsigned(L"1048576", 1, 1048576) == 1048576, "Upper bound");
        for (const auto *bad : {L"", L"-1", L"+1", L"0", L"1x", L"1048577", L"999999999999999999"})
            rejects([&] { parseUnsigned(bad, 1, 1048576); });
        const auto config = renderConfig(4096);
        require(config.find("r.Streaming.PoolSize=4096") != std::string::npos, "Pool substitution");
        require(config.find("{POOLSIZE}") == std::string::npos, "Unresolved marker");
        rejects([] { renderConfig(0); });
        require(fs::create_directory(root), "Fresh fixture directory");
        const auto target = root / L"Engine-\u00fc.ini";
        require(writeConfig(target, "original").empty(), "New file backup");
        const auto backup1 = writeConfig(target, config);
        require(read(target) == config && read(backup1) == "original", "First replacement");
        const auto backup2 = writeConfig(target, "second");
        require(backup1 != backup2 && read(backup1) == "original" && read(backup2) == config,
                "Backups must not overwrite earlier versions");
        rejects([&] { writeConfig(root / L"missing" / L"Engine.ini", config); });
        rejects([&] { writeConfig(root, config); });
        require(SetFileAttributesW(target.c_str(), FILE_ATTRIBUTE_READONLY), "Read-only fixture");
        rejects([&] { writeConfig(target, "must not replace"); });
        require(read(target) == "second", "Failed write changed original");
        SetFileAttributesW(target.c_str(), FILE_ATTRIBUTE_NORMAL);
        HANDLE locked = CreateFileW(target.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL, nullptr);
        require(locked != INVALID_HANDLE_VALUE, "Lock original for backup-failure test");
        bool backupFailed = false;
        try
        {
            writeConfig(target, "must not replace after failed backup");
        }
        catch (const std::exception &)
        {
            backupFailed = true;
        }
        CloseHandle(locked);
        require(backupFailed && read(target) == "second", "Backup failure changed original");
        for (const auto &file : fs::directory_iterator(root))
            require(file.path().extension() != L".tmp", "Temporary file leak");
        fs::remove_all(root);
        std::cout << "Configuration regression checks passed\n";
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        std::error_code ignored;
        fs::remove_all(root, ignored);
        return 1;
    }
}
