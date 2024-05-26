project('dasscc', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

dasscc = library('dasscc', [
    './src/dasscc/matrix.cc'
  ], install: true,
  include_directories: './include')

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dasscc)
