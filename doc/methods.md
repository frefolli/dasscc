# Methods to be implemented

| Notation | Meaning | Notes |
| -------- | ------- | ----- |
| \f$r_k\f$ | \f$b - Ax_k\f$ | also called residual of \f$x_k\f$ |
| \f$Vector::zeros(M)\f$ | \f$[0, 0, ... 0]\f$ | denotes a length M vector of zeros |
| \f$MAX\_ITER\f$ | Maximum number of iterations | must be set at compile time with a value > 20000 |
| \f$Q\f$ | \f$Q = P^-1\f$ | |
| \f$r_n\f$ | \f$r_n = r_{k+1}\f$ | |

## Direct

### Triangulars

#### Backward Substitution

```Rust
fn backward_substitution(A: Matrix<N, M>, b: Vector<M>) -> Result<Vector<M>, ()> {
  x = Vector::zeros(M);
  if A[M - 1, M - 1] == 0 {
    Err(())
  }
  x[M - 1] = b[M - 1] / A[M - 1, M - 1];
  for i in M - 2 .. 0 {
    if A[i, i] == 0 {
      Err(())
    }
    x[i] = (b[i] - (A[i,:] x)) / A[i, i];
  }
  Ok(x)
}
```

#### Forward Substitution

```Rust
fn forward_substitution(A: Matrix<N, M>, b: Vector<M>) -> Result<Vector<M>, ()> {
  x = Vector::zeros(M);
  if A[0, 0] == 0 {
    Err(())
  }
  x[0] = b[0] / A[0, 0];
  for i in 1 .. M - 1 {
    if A[i, i] == 0 {
      Err(())
    }
    x[i] = (b[i] - (A[i,:] x)) / A[i, i];
  }
  Ok(x)
}
```

## Iterative

### Statics

#### Jacobi

```Rust
fn jacobi(A: Matrix<N, M>, b: Vector<M>, tol: Float) -> Result<Vector<M>, Vector<M>> {
  x_0 = Vector::zeros(M);
  r_0 = b - A * x_0;
  for k in 0 .. MAX_ITER {
    if norm(r_k) / norm(b) < tol {
      Ok(x_k)
    }
    y = Q * r_k;
    x_n = x_k + y;
    r_n = b - A * x_n;
  }
  Err(x_last)
}
```

#### Gauss-Seidel

```Rust
fn gauss_seidel(A: Matrix<N, M>, b: Vector<M>, tol: Float) -> Result<Vector<M>, Vector<M>> {
  x_0 = Vector::zeros(M);
  r_0 = b - A * x_0;
    for k in 0 .. MAX_ITER {
    if norm(r_k) / norm(b) < tol {
      Ok(x_k)
    }
    y = forward_substitution(P, r_k);
    x_n = x_k + y;
    r_n = b - A * x_n;
  Err(x_last)
}
```

#### Richardson

```Rust
fn richardson(A: Matrix<N, M>, b: Vector<M>, tol: Float) -> Result<Vector<M>, Vector<M>> {
  x_0 = Vector::zeros(M);
  r_0 = b - A * x_0;
  alpha = 2 / (smallest_eigenvalue + biggest_eigenvalue);
  for k in 0 .. MAX_ITER {
    if norm(r_k) / norm(b) < tol {
      Ok(x_k)
    }
    y = alpha * Q * r_k;
    x_n = x_k + y;
    r_n = b - A * x_n;
  }
  Err(x_last)
```

### Dynamics

#### Gradient

```Rust
fn gradient(A: Matrix<N, M>, b: Vector<M>, tol: Float) -> Result<Vector<M>, Vector<M>> {
  x_0 = Vector::zeros(M);
  r_0 = b - Ax_0;
  for k in 0 .. MAX_ITER {
    if norm(r_k) / norm(b) < tol {
      Ok(x_k)
    }
    alpha_k = (r_k.transpose() * r_k) / (r_k.transpose() * A * r_k);
    y = alpha_k * r_k;
    x_n = x_k + y;
    r_n = b - A * x_n;
  }
  Err(x_last)
}
```

#### Conjugate Gradient

```Rust
fn conjugate_gradient(A: Matrix<N, M>, b: Vector<M>, tol: Float) -> Result<Vector<M>, Vector<M>> {
  x_0 = Vector::zeros(M);
  r_0 = b - A * x_0;
  d_0 = r_0;
  z_0 = A * r_0;
  v_0 = A * d_0;
  for k in 0 .. MAX_ITER {
    if norm(r_k) / norm(b) < tol {
      Ok(x_k)
    }
    alpha_k = (d_k.transpose() * r_k) / (r_k.transpose() * v_k);
    y = alpha_k * d_k;
    x_n = x_k + y;
    r_n = b - A * x_n;
    z_n = A * r_n;
    beta_k = (d_k.transpose() * z_n) / (d_k.transpose() * v_k);
    d_n = r_n - beta_k * d_k;
    v_n = A * d_n;
  }
  Err(x_last)
}
```
