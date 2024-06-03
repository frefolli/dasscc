project('dasscc', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

eigen3 = dependency('eigen3')
jsoncpp = dependency('jsoncpp')

dasscc = library('dasscc', [
    'src/dasscc/backward_substitution_solver.cc',
    'src/dasscc/forward_substitution_solver.cc',
    'src/dasscc/gauss_elimination_solver.cc',
    'src/dasscc/conjugate_gradient_engine.cc',
    'src/dasscc/richardson_engine.cc',
    'src/dasscc/jacobi_engine.cc',
    'src/dasscc/gradient_engine.cc',
    'src/dasscc/gauss_seidel_engine.cc',
    'src/dasscc/logging.cc',
    'src/dasscc/matrix.cc',
    'src/dasscc/testing.cc',
    'src/dasscc/state.cc',
    'src/dasscc/specifiers.cc',
    'src/dasscc/timer.cc',
    'src/dasscc/benchmark.cc',
  ], install: true,
  include_directories: ['./include'],
  dependencies: [eigen3, jsoncpp])

test_conjugate_gradient_engine = executable('test_conjugate_gradient_engine', [
    'tests/src/dasscc/conjugate_gradient_engine.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Conjugate Gradient Engine Test', test_conjugate_gradient_engine)

test_richardson_engine = executable('test_richardson_engine', [
    'tests/src/dasscc/richardson_engine.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Richardson Engine Test', test_richardson_engine)

test_gauss_elimination = executable('test_gauss_elimination', [
    'tests/src/dasscc/gauss_elimination.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Gauss Elimination Test', test_gauss_elimination)

test_forward_substitution = executable('test_forward_substitution', [
    'tests/src/dasscc/forward_substitution.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Forward Substitution Test', test_forward_substitution)

test_jacobi_engine = executable('test_jacobi_engine', [
    'tests/src/dasscc/jacobi_engine.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Jacobi Engine Test', test_jacobi_engine)

test_gradient_engine = executable('test_gradient_engine', [
    'tests/src/dasscc/gradient_engine.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Gradient Engine Test', test_gradient_engine)

test_gauss_seidel_engine = executable('test_gauss_seidel_engine', [
    'tests/src/dasscc/gauss_seidel_engine.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Gauss Seidel Engine Test', test_gauss_seidel_engine)

test_backward_substitution = executable('test_backward_substitution', [
    'tests/src/dasscc/backward_substitution.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)
test('Backward Substitution Test', test_backward_substitution)

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dasscc)
