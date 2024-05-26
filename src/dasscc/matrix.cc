#include <dasscc/matrix.hh>
#include <random>
#include <cassert>
#include <vector>
#include <set>
#include <fstream>
#include <eigen3/unsupported/Eigen/src/SparseExtra/MarketIO.h>

bool dasscc::LoadFromFile(Eigen::SparseMatrix<double_t> &matrix, const std::string& filepath) {
  return (Eigen::loadMarket(matrix, filepath));
}

bool dasscc::DumpToFile(const Eigen::SparseMatrix<double_t> &matrix, const std::string& filepath) {
  return (Eigen::saveMarket(matrix, filepath));
}

void dasscc::Random(Eigen::SparseMatrix<double_t> &matrix, uint32_t N, uint32_t M, double_t density) {
  typedef Eigen::Triplet<double> T;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> valdis(0, 1.0);
  std::uniform_int_distribution<> rowdis(0, N-1);
  std::uniform_int_distribution<> coldis(0, M-1);

  std::vector<Eigen::Triplet<double> > tripletList;
  uint32_t nnz = (uint32_t) (N * (M * density));
  std::set<size_t> nnz_pos;
  for (uint32_t i = 0; i < nnz; ++i) {
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

  matrix.resize(N, M);
  matrix.setFromTriplets(tripletList.begin(), tripletList.end());
}

void dasscc::RandomSymmetricPositiveDefined(Eigen::SparseMatrix<double_t> &matrix, uint32_t N, double_t density) {
  Eigen::SparseMatrix<double_t> Q;
  dasscc::Random(Q, N, N, density);
  matrix = (Eigen::SparseMatrix<double_t>(Q.transpose()) * Q);
  for (uint32_t i = 0; i < N; i++)
    matrix.coeffRef(i, i) += 1;
}

bool dasscc::IsSymmetricPositiveDefined(const Eigen::SparseMatrix<double_t> &matrix) {
  // TODO:
  return false;
}

void dasscc::ArrayOfZeros(Eigen::SparseMatrix<double_t> &matrix, uint32_t N) {
  // TODO:
}

void dasscc::ArrayOfOnes(Eigen::SparseMatrix<double_t> &matrix, uint32_t N) {
  // TODO:
}