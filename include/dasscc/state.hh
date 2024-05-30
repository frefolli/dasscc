#ifndef DASSCC_STATE_HH
#define DASSCC_STATE_HH
/** @file */
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
#include <cmath>
namespace dasscc {
  /** unfortunately we don't have a powerful macro system */
  struct State {
    Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
    Eigen::SparseVector<double_t> b;
    Eigen::SparseVector<double_t> y;
    Eigen::SparseMatrix<double_t, Eigen::RowMajor> P;
    Eigen::SparseMatrix<double_t, Eigen::RowMajor> Q;
    Eigen::SparseVector<double_t> x_k;
    Eigen::SparseVector<double_t> x_n;
    Eigen::SparseVector<double_t> r_k;
    Eigen::SparseVector<double_t> r_n;
    Eigen::SparseVector<double_t> d_k;
    Eigen::SparseVector<double_t> d_n;
    Eigen::SparseVector<double_t> z_k;
    Eigen::SparseVector<double_t> z_n;
    Eigen::SparseVector<double_t> v_k;
    Eigen::SparseVector<double_t> v_n;
    double_t alpha_k;
    double_t beta_k;
    double_t alpha;

    /** _n goes into _k */
    void update();
  };
}
#endif//DASSCC_STATE_HH