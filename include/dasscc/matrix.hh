#ifndef DASSCC_MATRIX_HH
#define DASSCC_MATRIX_HH
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file matrix.hh Matrix Utils */
namespace dasscc {
  /**
   * Loads a Sparse Matrix from .mtx file
   * @param matrix output matrix
   * @param filepath input file path
   */
  void LoadFromFile(Eigen::SparseMatrix<double_t> &matrix, const std::string& filepath);
  
  /**
   * Dumps a Sparse Matrix from .mtx file
   * @param matrix input matrix
   * @param filepath output file path
   */
  void DumpToFile(const Eigen::SparseMatrix<double_t> &matrix, const std::string& filepath);
  
  /**
   * Sets the input matrix to a new random NxM sparse matrix;
   */
  void Random(Eigen::SparseMatrix<double_t> &matrix, int N, int M, double_t density);
  
  /**
   * Sets the input matrix to a new random NxN symmetric and positive defined sparse matrix;
   */
  void RandomSymmetricPositiveDefined(Eigen::SparseMatrix<double_t> &matrix, int N, double_t density);
  
  /**
   * Returns true if the input matrix is symmetric and positive defined.
   */
  bool IsSymmetricPositiveDefined(const Eigen::SparseMatrix<double_t> &matrix);
  
  /**
   * Set matrix to a Nx1 array of 0s.
   */
  void ArrayOfZeros(Eigen::SparseMatrix<double_t> &matrix, int N);
  
  /**
   * Set matrix to a Nx1 array of 1s.
   */
  void ArrayOfOnes(Eigen::SparseMatrix<double_t> &matrix, int N);
};
#endif//DASSCC_MATRIX_HH