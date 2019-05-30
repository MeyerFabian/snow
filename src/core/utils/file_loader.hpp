#ifndef FILE_LOADER
#define FILE_LOADER
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
struct FileSystem {
  static std::string load_string_from_file(const std::string& filename);
  static void write_to_file(std::string source, const std::string& filename);
};
#endif

