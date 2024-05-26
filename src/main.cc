#include <dasscc/matrix.hh>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <set>
#include <string>
#include <filesystem>

std::string FilenameForSPDMatrix(uint32_t N, uint32_t M) {
  return "resources/matrices/spd/" + std::to_string(N) + "x" + std::to_string(M) + ".mtx";
}

int main(int argc, char** args) {
  uint32_t N;
  if (argc > 1) {
    N = std::stoi(args[1]);
  } else {
    N = 50;
  }

  Eigen::SparseMatrix<double_t> mat;
  std::string filename = FilenameForSPDMatrix(N, N);
  if (std::filesystem::exists(filename)) {
    if (dasscc::LoadFromFile(mat, filename)) {
      std::cerr << "Matrix " << N << "x" << N << " loaded successfuly" << std::endl;
      std::cout << mat << std::endl;
    } else {
      std::cerr << "Unable to load matrix " << N << "x" << N << std::endl;
    }
  } else {
    dasscc::RandomSymmetricPositiveDefined(mat, N, 0.05);
    if (dasscc::DumpToFile(mat, filename))
      std::cerr << "Matrix " << N << "x" << N << " saved successfuly" << std::endl;
    else
      std::cerr << "Unable to save matrix " << N << "x" << N << std::endl;
  }
}
