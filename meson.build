project('dasscc', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

dasscc = library('dasscc', [
    'src/dasscc/backward_substitution_solver.cc',
    'src/dasscc/forward_substitution_solver.cc',
    'src/dasscc/jacobi_engine.cc',
    'src/dasscc/logging.cc',
    'src/dasscc/matrix.cc',
    'src/dasscc/state.cc',
  ], install: true,
  include_directories: ['./include'])

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dasscc)
