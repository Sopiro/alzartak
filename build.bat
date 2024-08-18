rmdir /s /q build
mkdir build
pushd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G "Ninja Multi-Config"
cmake --build . --config Release --parallel
popd