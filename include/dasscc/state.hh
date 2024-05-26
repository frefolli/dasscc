#ifndef DASSCC_STATE_HH
#define DASSCC_STATE_HH
/** @file */
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
#include <cmath>
namespace dasscc {
  /** unfortunately we don't have a powerful macro system */
  struct State {
    Eigen::SparseMatrix<double_t> A;
    Eigen::SparseMatrix<double_t> b;
    Eigen::SparseMatrix<double_t> y;
    Eigen::SparseMatrix<double_t> P;
    Eigen::SparseMatrix<double_t> Q;
    Eigen::SparseMatrix<double_t> x_k;
    Eigen::SparseMatrix<double_t> x_n;
    Eigen::SparseMatrix<double_t> r_k;
    Eigen::SparseMatrix<double_t> r_n;
    Eigen::SparseMatrix<double_t> d_k;
    Eigen::SparseMatrix<double_t> d_n;
    Eigen::SparseMatrix<double_t> z_k;
    Eigen::SparseMatrix<double_t> z_n;
    Eigen::SparseMatrix<double_t> v_k;
    Eigen::SparseMatrix<double_t> v_n;
    double_t alpha_k;
    double_t beta_k;
    double_t alpha;

    /** _n goes into _k */
    void update();
  };
}
#endif//DASSCC_STATE_HH