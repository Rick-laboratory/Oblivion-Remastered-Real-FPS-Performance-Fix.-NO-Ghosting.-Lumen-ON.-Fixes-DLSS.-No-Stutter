#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

unsigned parseUnsigned(std::wstring_view value, unsigned minimum, unsigned maximum);
unsigned poolFromDedicatedBytes(std::uint64_t bytes);
std::string renderConfig(unsigned poolMiB);
// Replaces the whole file. Existing contents are backed up first; the returned path is empty for new files.
std::filesystem::path writeConfig(const std::filesystem::path &target, std::string_view contents);
