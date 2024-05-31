project('dasscc', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

dasscc = library('dasscc', [
    'src/dasscc/backward_substitution_solver.cc',
    'src/dasscc/forward_substitution_solver.cc',
    'src/dasscc/gauss_elimination_solver.cc',
    'src/dasscc/jacobi_engine.cc',
    'src/dasscc/logging.cc',
    'src/dasscc/matrix.cc',
    'src/dasscc/testing.cc',
    'src/dasscc/state.cc',
  ], install: true,
  include_directories: ['./include'])

tests = executable('tests', [
    'tests/src/dasscc/conjugate_gradient_engine.cc',
    'tests/src/dasscc/richardson_engine.cc',
    'tests/src/dasscc/gauss_elimination.cc',
    'tests/src/dasscc/forward_substitution.cc',
    'tests/src/dasscc/jacobi_engine.cc',
    'tests/src/dasscc/gradient_engine.cc',
    'tests/src/dasscc/gauss_seidel_engine.cc',
    'tests/src/dasscc/backward_substitution.cc',
    'tests/src/main.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dasscc)

test('tests', tests)

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dasscc)
