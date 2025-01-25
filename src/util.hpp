#pragma once

#include <cstdint>
#include <fstream>
#include <vector>
#include <filesystem>

std::ifstream getInputFile(const std::filesystem::path& path);
std::ofstream getOutputFile(const std::filesystem::path& path);
std::vector<uint8_t> getBytes(std::ifstream file, uint64_t start, uint64_t limit, uint64_t end);