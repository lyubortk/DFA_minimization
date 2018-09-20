make
./dfa_minimizer $1 output.dot $2
dot -Gdpi=200 -Tpng output.dot -o result.png
rm output.dot
