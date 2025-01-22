#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

std::ifstream getInputFile(const std::string& path);
std::ofstream getOutputFile(const std::string& path);
std::vector<uint8_t> getBytes(std::ifstream file);