#include "config.h"
#include "gpu.h"
#include <algorithm>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace
{
void usage()
{
    std::cout << "Oblivion configuration tool\n"
                 "  --list-gpus\n"
                 "  --preview [--pool-mib N | --adapter N]\n"
                 "  --apply PATH [--pool-mib N | --adapter N]\n\n"
                 "PATH is an Engine.ini in an existing directory. --apply replaces the\n"
                 "whole file with the bundled historical preset after making a unique backup.\n"
                 "Without a pool override, the original 95% dedicated-VRAM heuristic is used.\n";
}
} // namespace
int wmain(int argc, wchar_t *argv[])
{
    try
    {
        if (argc == 1 || (argc == 2 && std::wstring_view(argv[1]) == L"--help"))
        {
            usage();
            return 0;
        }
        bool preview = false, list = false, apply = false;
        std::filesystem::path target;
        std::optional<unsigned> pool, adapter;
        for (int i = 1; i < argc; ++i)
        {
            const std::wstring_view arg(argv[i]);
            auto value = [&]() -> std::wstring_view {
                if (++i == argc)
                    throw std::invalid_argument("Missing option value.");
                return argv[i];
            };
            if (arg == L"--preview" && !preview)
                preview = true;
            else if (arg == L"--list-gpus" && !list)
                list = true;
            else if (arg == L"--apply" && !apply)
            {
                apply = true;
                target = value();
            }
            else if (arg == L"--pool-mib" && !pool)
                pool = parseUnsigned(value(), 1, 1048576);
            else if (arg == L"--adapter" && !adapter)
                adapter = parseUnsigned(value(), 0, 1024);
            else
                throw std::invalid_argument("Unknown or repeated option.");
        }
        if (static_cast<int>(preview) + static_cast<int>(list) + static_cast<int>(apply) != 1 ||
            (pool && adapter) || (list && (pool || adapter)))
            throw std::invalid_argument("Choose one mode and at most one pool/adapter override.");

        if (!pool || list)
        {
            const auto gpus = enumerateGpus();
            if (list)
            {
                for (const auto &gpu : gpus)
                    std::wcout << gpu.index << L": " << gpu.name << L" - "
                               << gpu.dedicatedBytes / (1024 * 1024) << L" MiB dedicated\n";
                return gpus.empty() ? 1 : 0;
            }
            const auto selected =
                adapter ? std::find_if(gpus.begin(), gpus.end(),
                                       [&](const auto &gpu) { return gpu.index == *adapter; })
                        : std::max_element(gpus.begin(), gpus.end(), [](const auto &a, const auto &b) {
                              return a.dedicatedBytes < b.dedicatedBytes;
                          });
            if (selected == gpus.end())
                throw std::runtime_error("No matching GPU.");
            pool = poolFromDedicatedBytes(selected->dedicatedBytes);
            std::wcerr << L"GPU: " << selected->name << L"\n";
        }
        const auto config = renderConfig(*pool);
        std::cerr << "Texture pool: " << *pool << " MiB\n";
        if (preview)
            std::cout << config;
        else
        {
            const auto backup = writeConfig(target, config);
            std::wcout << L"Wrote: " << target.wstring() << L"\n";
            if (!backup.empty())
                std::wcout << L"Backup: " << backup.wstring() << L"\n";
        }
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << "\nUse --help for usage.\n";
        return 1;
    }
}
