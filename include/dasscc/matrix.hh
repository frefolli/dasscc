#ifndef DASSCC_MATRIX_HH
#define DASSCC_MATRIX_HH
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
#include <cmath>
#include <cstdint>
/** @file matrix.hh Matrix Utils */
namespace dasscc {
  /**
   * Loads a Sparse Matrix from .mtx file
   * @param matrix output matrix
   * @param filepath input file path
   */
  bool LoadFromFile(Eigen::SparseMatrix<double_t>& matrix, const std::string& filepath);

  /**
   * Dumps a Sparse Matrix from .mtx file
   * @param matrix input matrix
   * @param filepath output file path
   */
  bool DumpToFile(const Eigen::SparseMatrix<double_t>& matrix, const std::string& filepath);

  /**
   * Sets the input matrix to a new random NxM sparse matrix;
   */
  void Random(Eigen::SparseMatrix<double_t>& matrix, uint32_t N, uint32_t M, double_t density);

  /**
   * Sets the input matrix to a new random NxN upper triangular;
   */
  void RandomUpperTriangular(Eigen::SparseMatrix<double_t>& matrix, uint32_t N, uint32_t M, double_t density);

  /**
   * Sets the input matrix to a new random NxN lower triangular;
   */
  void RandomLowerTriangular(Eigen::SparseMatrix<double_t>& matrix, uint32_t N, uint32_t M, double_t density);

  /**
   * Sets the input matrix to a new random NxN symmetric and positive defined sparse matrix;
   */
  void RandomSymmetricPositiveDefined(Eigen::SparseMatrix<double_t>& matrix, uint32_t N, double_t density);

  /**
   * Sets the input matrix to a new random NxN diagonal dominant sparse matrix;
   */
  void RandomDiagonalDominant(Eigen::SparseMatrix<double_t>& matrix, uint32_t N, double_t density);

  /**
   * Returns true if the input matrix is symmetric and positive defined.
   */
  bool IsSymmetricPositiveDefined(const Eigen::SparseMatrix<double_t>& matrix);

  /**
   * Computes eigenvalues of a Sparse Matrix into a Vector of double_t
  */
  bool Eigenvalues(Eigen::MatrixXd& eigenvalues, const Eigen::SparseMatrix<double_t>& matrix);

  /**
   * Computes smallest and biggest eigenvalues of a Sparse Matrix
  */
  bool Eigenvalues(double_t& smallest, double_t& biggest, const Eigen::SparseMatrix<double_t>& matrix);

  /**
   * Set matrix to a Nx1 array of 0s.
   */
  void ArrayOfZeros(Eigen::SparseVector<double_t>& matrix, uint32_t N);

  /**
   * Set matrix to a Nx1 array of 1s.
   */
  void ArrayOfOnes(Eigen::SparseVector<double_t>& matrix, uint32_t N);

  /**
   * Set sparse matrix to a NxN identity.
   */
  void Identity(Eigen::SparseMatrix<double_t>& matrix, uint32_t N);
};
#endif//DASSCC_MATRIX_HH