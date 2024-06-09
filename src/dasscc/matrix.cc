#include <cstdlib>
#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <filesystem>
#include <random>
#include <cassert>
#include <sstream>
#include <vector>
#include <set>
#include <fstream>
#include <eigen3/unsupported/Eigen/src/SparseExtra/MarketIO.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/Util/CompInfo.h>
const std::string CACHEDIR = "/home/refo/.cache/dasscc";

bool dasscc::LoadFromFile(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filepath) {
  return (Eigen::loadMarket(matrix, filepath));
}

bool dasscc::DumpToFile(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filepath) {
  return (Eigen::saveMarket(matrix, filepath));
}

bool dasscc::DumpToBin(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filename) {
  assert(matrix.isCompressed() == true);
  std::ofstream out(filename, std::ios::binary | std::ios::out | std::ios::trunc);
  if(out.is_open()) {
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index rows, cols, nnzs, outS, innS;
    rows = matrix.rows()     ;
    cols = matrix.cols()     ;
    nnzs = matrix.nonZeros() ;
    outS = matrix.outerSize();
    innS = matrix.innerSize();

    out.write(reinterpret_cast<char*>(&rows), sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index));
    out.write(reinterpret_cast<char*>(&cols), sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index));
    out.write(reinterpret_cast<char*>(&nnzs), sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index));
    out.write(reinterpret_cast<char*>(&outS), sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index));
    out.write(reinterpret_cast<char*>(&innS), sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index));

    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index sizeIndexS = static_cast<typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index>(sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::StorageIndex));
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index sizeScalar = static_cast<typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index>(sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Scalar      ));
    out.write(reinterpret_cast<const char*>(matrix.valuePtr()),       sizeScalar * nnzs);
    out.write(reinterpret_cast<const char*>(matrix.outerIndexPtr()),  sizeIndexS  * outS);
    out.write(reinterpret_cast<const char*>(matrix.innerIndexPtr()),  sizeIndexS  * nnzs);

    out.close();
    return true;
  }
  return false;
}

bool dasscc::LoadFromBin(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const std::string& filename) {
  std::ifstream in(filename, std::ios::binary | std::ios::in);
  if(in.is_open()) {
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index rows, cols, nnz, inSz, outSz;
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index sizeScalar = static_cast<typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index>(sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Scalar      ));
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index sizeIndex  = static_cast<typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index>(sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index       ));
    typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index sizeIndexS = static_cast<typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::Index>(sizeof(typename Eigen::SparseMatrix<double_t, Eigen::RowMajor>::StorageIndex));
    in.read(reinterpret_cast<char*>(&rows ), sizeIndex);
    in.read(reinterpret_cast<char*>(&cols ), sizeIndex);
    in.read(reinterpret_cast<char*>(&nnz  ), sizeIndex);
    in.read(reinterpret_cast<char*>(&outSz), sizeIndex);
    in.read(reinterpret_cast<char*>(&inSz ), sizeIndex);

    matrix.resize(rows, cols);
    matrix.makeCompressed();
    matrix.resizeNonZeros(nnz);

    in.read(reinterpret_cast<char*>(matrix.valuePtr())     , sizeScalar * nnz  );
    in.read(reinterpret_cast<char*>(matrix.outerIndexPtr()), sizeIndexS * outSz);
    in.read(reinterpret_cast<char*>(matrix.innerIndexPtr()), sizeIndexS * nnz );

    matrix.finalize();
    in.close();
    return true;
  }
  return false;
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
  biggest = found.coeff(0, 0).real();
  smallest = found.coeff(rows - 1, 0).real();
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

double_t dasscc::ConditionNumber(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix) {
  double_t smallest, biggest;
  dasscc::Eigenvalues(smallest, biggest, matrix);
  return biggest / smallest;
}

inline std::string MangledPathForConditionNumber(const dasscc::MatrixSpecifier& specifier) {
  std::filesystem::create_directories(CACHEDIR);
  std::ostringstream str;
  str << CACHEDIR << "/" << dasscc::ToString(specifier.type) << "_" << specifier.ID << "_" << specifier.N << "_" << specifier.density << ".cn.bin";
  return str.str();
}

double_t dasscc::ConditionNumber(const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const dasscc::MatrixSpecifier& specifier) {
  std::string mangled_path = MangledPathForConditionNumber(specifier);
  double_t condition_number = -1.0f;
  
  std::ifstream in(mangled_path, std::ios::binary | std::ios::in);
  if(in.is_open()) {
    in.read(reinterpret_cast<char*>(&condition_number), sizeof(double_t));
    in.close();
  } else {
    condition_number = dasscc::ConditionNumber(matrix);
    std::ofstream out(mangled_path, std::ios::binary | std::ios::out | std::ios::trunc);
    assert(out.is_open());
    out.write(reinterpret_cast<char*>(&condition_number), sizeof(double_t));
    out.close();
  }
  return condition_number;
}

bool dasscc::FromMatrixSpecifier(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, std::string pattern) {
  dasscc::MatrixSpecifier specifier = dasscc::ParseMatrixSpecifier(pattern);
  return dasscc::FromMatrixSpecifier(matrix, specifier);
}

inline std::string MangledPathForCache(const dasscc::MatrixSpecifier& specifier) {
  std::filesystem::create_directories(CACHEDIR);
  std::ostringstream str;
  str << CACHEDIR << "/" << dasscc::ToString(specifier.type) << "_" << specifier.ID << "_" << specifier.N << "_" << specifier.density << ".bin";
  return str.str();
}

bool dasscc::FromMatrixSpecifier(Eigen::SparseMatrix<double_t, Eigen::RowMajor>& matrix, const dasscc::MatrixSpecifier& specifier) {
  bool success = true;
  std::string mangled_path = MangledPathForCache(specifier);
  if (!dasscc::LoadFromBin(matrix, mangled_path)) {
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
    matrix.makeCompressed();
    assert(dasscc::DumpToBin(matrix, mangled_path));
  }
  return success;
}
