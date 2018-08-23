#include "file_loader.hpp"
std::string FileSystem::load_string_from_file(const std::string& filename) {
  std::ifstream file;

  file.open(filename.c_str());
  if (!file.good()) {
    std::cerr << "could not open file: " << filename.c_str() << std::endl;
    exit(-1);
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();
  return stream.str();
}
void FileSystem::write_to_file(std::string source,
                               const std::string& filename) {
  std::ofstream file;
  file.open(filename);
  file << source;
  file.close();
}

