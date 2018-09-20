make
./dfa_minimizer $1 output.dot
dot -Gdpi=200 -Tpng output.dot -o result.png
rm output.dot
