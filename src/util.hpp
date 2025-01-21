#pragma once

#include <fstream>
#include <vector>

std::ifstream getInputFile(const std::string& path);
std::ofstream getOutputFile(const std::string& path);
std::vector<uint8_t> getBytes(std::ifstream file);