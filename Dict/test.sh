./testexact
./testexact_s
valgrind ./testexact
./spellexact eng_370k_shuffle.txt p-and-p-words.txt
./spellexact_s eng_370k_shuffle.txt p-and-p-words.txt
valgrind ./spellexact eng_370k_shuffle.txt p-and-p-words.txt

./testapprox_s
valgrind ./testapprox
./spellapprox eng_370k_shuffle.txt p-and-p-words.txt
./spellapprox_s eng_370k_shuffle.txt p-and-p-words.txt
valgrind ./spellapprox eng_370k_shuffle.txt p-and-p-words.txt

