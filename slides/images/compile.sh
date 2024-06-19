function compile_d2() {
  IN=$1
  d2 $IN.d2 $IN.png
}

compile_d2 cd-cge
compile_d2 cd-gre
compile_d2 cd-gse
compile_d2 cd-jae
