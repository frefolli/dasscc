#include <iostream>
#include <fstream>
#include <cmath>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
#include <eigen3/unsupported/Eigen/src/SparseExtra/MarketIO.h>
#include <random>
#include <set>
#include <string>
#include <filesystem>

void RandomSparseMatrix(Eigen::SparseMatrix<double_t> &out, int N, int M, double_t density) {
  typedef Eigen::Triplet<double> T;
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> valdis(0, 1.0);
  std::uniform_int_distribution<> rowdis(0, N-1);
  std::uniform_int_distribution<> coldis(0, M-1);

  std::vector<Eigen::Triplet<double> > tripletList;
  size_t nnz = (size_t) (N * (M * density));
  std::set<size_t> nnz_pos;
  for (size_t i = 0; i < nnz; ++i) {
    auto r = rowdis(gen);
    auto c = coldis(gen);
    size_t pos = r * M + c;
    while (nnz_pos.find(pos) != nnz_pos.end()) {
      r = rowdis(gen);
      c = coldis(gen);
      pos = r * M + c;
    }

    nnz_pos.insert(pos);
    tripletList.push_back(T(r, c, valdis(gen)));
  }

  out.resize(N, M);
  out.setFromTriplets(tripletList.begin(), tripletList.end());   //create the matrix
}

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
    if (Eigen::loadMarket(mat, filename)) {
      std::cerr << "Matrix " << N << "x" << N << " loaded successfuly" << std::endl;
      std::cout << mat << std::endl;
    } else {
      std::cerr << "Unable to load matrix " << N << "x" << N << std::endl;
    }
  } else {
    Eigen::SparseMatrix<double_t> Q;
    RandomSparseMatrix(Q, N, N, 0.05);
    mat = (Eigen::SparseMatrix<double_t>(Q.transpose()) * Q);
    for (uint32_t i = 0; i < N; i++)
      mat.coeffRef(i, i) += 1;
    if (Eigen::saveMarket(mat, filename))
      std::cerr << "Matrix " << N << "x" << N << " saved successfuly" << std::endl;
    else
      std::cerr << "Unable to save matrix " << N << "x" << N << std::endl;
  }
}
