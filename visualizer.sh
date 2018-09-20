./dfa_minimizer "$1" output.dot $3
dot -Gdpi=200 -Tpng output.dot -o "$2"
rm output.dot
