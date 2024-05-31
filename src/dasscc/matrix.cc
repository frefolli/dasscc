#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <random>
#include <cassert>
#include <vector>
#include <set>
#include <fstream>
#include <eigen3/unsupported/Eigen/src/SparseExtra/MarketIO.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/Util/CompInfo.h>

bool dasscc::LoadFromFile(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filepath) {
  return (Eigen::loadMarket(matrix, filepath));
}

bool dasscc::DumpToFile(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filepath) {
  return (Eigen::saveMarket(matrix, filepath));
}

void dasscc::Random(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, uint32_t M, double_t density) {
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
    double_t val = valdis(gen);
    while(val == 0.0)
      val = valdis(gen);
    tripletList.push_back(Eigen::Triplet<double>(r, c, val));
  }

  matrix.resize(N, M);
  matrix.setFromTriplets(tripletList.begin(), tripletList.end());
}

void dasscc::RandomLowerTriangular(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, uint32_t M, double_t density) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> valdis(0, 1.0);
  std::uniform_int_distribution<> rowdis(0, N-1);
  std::uniform_int_distribution<> coldis(0, M-1);

  std::vector<Eigen::Triplet<double> > tripletList;
  uint32_t nnz = (uint32_t) (N * (M * density) * 0.5);
  std::set<size_t> nnz_pos;
  for (uint32_t i = 0; i < nnz; ++i) {
    auto r = rowdis(gen);
    auto c = coldis(gen) % (r + 1);
    size_t pos = r * M + c;
    while (nnz_pos.find(pos) != nnz_pos.end()) {
      r = rowdis(gen);
      c = coldis(gen) % (r + 1);
      pos = r * M + c;
    }

    nnz_pos.insert(pos);
    tripletList.push_back(Eigen::Triplet<double>(r, c, valdis(gen)));
  }

  matrix.resize(N, M);
  matrix.setFromTriplets(tripletList.begin(), tripletList.end());
  uint32_t L = std::min(N, M);
  for (uint32_t i = 0; i < L; i++)
    matrix.coeffRef(i, i) += 1;
}

void dasscc::RandomUpperTriangular(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, uint32_t M, double_t density) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> valdis(0, 1.0);
  std::uniform_int_distribution<> rowdis(0, N-1);
  std::uniform_int_distribution<> coldis(0, M-1);

  std::vector<Eigen::Triplet<double> > tripletList;
  uint32_t nnz = (uint32_t) (N * (M * density) * 0.5);
  std::set<size_t> nnz_pos;
  for (uint32_t i = 0; i < nnz; ++i) {
    auto r = rowdis(gen);
    auto c = coldis(gen);
    while (c < r)
      c = coldis(gen);
    size_t pos = r * M + c;
    while (nnz_pos.find(pos) != nnz_pos.end()) {
      r = rowdis(gen);
      c = coldis(gen);
      while (c < r)
        c = coldis(gen);
      pos = r * M + c;
    }

    nnz_pos.insert(pos);
    double_t val = valdis(gen);
    while(val == 0.0)
      val = valdis(gen);
    tripletList.push_back(Eigen::Triplet<double>(r, c, val));
  }

  matrix.resize(N, M);
  matrix.setFromTriplets(tripletList.begin(), tripletList.end());
  uint32_t L = std::min(N, M);
  for (uint32_t i = 0; i < L; i++)
    matrix.coeffRef(i, i) += 1;
}

void dasscc::RandomSymmetricPositiveDefined(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, double_t density) {
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> Q;
  dasscc::Random(Q, N, N, density);
  matrix = (Eigen::SparseMatrix<double_t, Eigen::RowMajor>(Q.transpose()) * Q);
  for (uint32_t i = 0; i < N; i++)
    matrix.coeffRef(i, i) += 1;
}

void dasscc::RandomRowDiagonalDominant(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, double_t density) {
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> Q;
  dasscc::Random(Q, N, N, density);
  matrix = (Eigen::SparseMatrix<double_t, Eigen::RowMajor>(Q.transpose()) * Q);
  for (uint32_t i = 0; i < N; i++) {
    matrix.coeffRef(i, i) = (abs(matrix.row(i).sum()) + matrix.coeff(i, i) + 10) * 20;
  }
}

void dasscc::RandomColumnDiagonalDominant(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N, double_t density) {
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> Q;
  dasscc::Random(Q, N, N, density);
  matrix = (Eigen::SparseMatrix<double_t, Eigen::RowMajor>(Q.transpose()) * Q);
  for (uint32_t i = 0; i < N; i++) {
    matrix.coeffRef(i, i) = (abs(matrix.col(i).sum()) + matrix.coeff(i, i) + 10) * 20;
  }
}

bool dasscc::IsSymmetricPositiveDefined(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix) {
  Eigen::MatrixXd eigenvalues;
  if (!dasscc::Eigenvalues(eigenvalues, matrix))
    RaiseFatalError("unable to compute eigenvalues of possibly-PSD sparse matrix");
  const uint32_t rows = eigenvalues.rows();
  for (uint32_t i = 0; i < rows; i++) {
    if (eigenvalues.coeff(i, 0) <= 0)
      return false;
  }
  return true;
}

void dasscc::ArrayOfZeros(Eigen::SparseVector<double_t>& matrix, uint32_t N) {
  matrix.resize(N);
  matrix.data().squeeze();
}

void dasscc::ArrayOfOnes(Eigen::SparseVector<double_t>& matrix, uint32_t N) {
  matrix.resize(N);
  matrix.reserve(N);
  for (uint32_t i = 0; i < N; i++)
    matrix.coeffRef(i) = 1;
  matrix.data().squeeze(); 
}

void dasscc::Identity(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, uint32_t N) {
  matrix.resize(N, N);
  matrix.reserve(N);
  matrix.setIdentity();
  matrix.data().squeeze();
}

/**
 * Computes eigenvalues of a Sparse Matrix into a Vector of std::complex<double_t>
*/
inline bool _Eigenvalues(Eigen::MatrixXcd& eigenvalues, const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix) {
  uint32_t N = matrix.cols();
  Spectra::SparseSymMatProd<double_t, 1, Eigen::RowMajor> operating(matrix);
  Spectra::GenEigsSolver<decltype(operating)> solver(operating, N - 2, N);
  solver.init();
  (void)solver.compute();
  if(solver.info() == Spectra::CompInfo::Successful) {
    eigenvalues = solver.eigenvalues();
    return true;
  }
  return false;
}

bool dasscc::Eigenvalues(double_t& smallest, double_t& biggest, const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix) {
  Eigen::MatrixXcd found;
  if (!_Eigenvalues(found, matrix))
    return false;
  const uint32_t rows = found.rows();
  smallest = found.coeff(0, 0).real();
  biggest = found.coeff(rows - 1, 0).real();
  return true;
}

bool dasscc::Eigenvalues(Eigen::MatrixXd& eigenvalues, const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix) {
  Eigen::MatrixXcd found;
  if (!_Eigenvalues(found, matrix))
    return false;
  const uint32_t rows = found.rows();
  eigenvalues.resize(rows, 1);
  for (uint32_t i = 0; i < rows; i++)
    eigenvalues.coeffRef(i, 0) = found.coeff(i, 0).real();
  return true;
}

bool dasscc::FromMatrixSpecifier(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, std::string pattern) {
  dasscc::MatrixSpecifier specifier = dasscc::ParseMatrixSpecifier(pattern);
  return dasscc::FromMatrixSpecifier(matrix, specifier);
}
  
bool dasscc::FromMatrixSpecifier(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, dasscc::MatrixSpecifier& specifier) {
  bool success = true;
  switch (specifier.type) {
    case dasscc::MatrixSpecifier::Type::SPD: {
      dasscc::RandomSymmetricPositiveDefined(matrix, specifier.N, specifier.density);
    }; break;
    case dasscc::MatrixSpecifier::Type::UT: {
      dasscc::RandomUpperTriangular(matrix, specifier.N, specifier.N, specifier.density);
    }; break;
    case dasscc::MatrixSpecifier::Type::LT: {
      dasscc::RandomLowerTriangular(matrix, specifier.N, specifier.N, specifier.density);
    }; break;
    case dasscc::MatrixSpecifier::Type::CDD: {
      dasscc::RandomColumnDiagonalDominant(matrix, specifier.N, specifier.density);
    }; break;
    case dasscc::MatrixSpecifier::Type::RDD: {
      dasscc::RandomRowDiagonalDominant(matrix, specifier.N, specifier.density);
    }; break;
    case dasscc::MatrixSpecifier::Type::SRC: {
      std::string path = "resources/matrices/" + specifier.ID + ".mtx";
      success = dasscc::LoadFromFile(matrix, path);
    }; break;
    default: {
      success = false;
    };
  }
  return success;
}
