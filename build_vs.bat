rmdir /s /q build
mkdir build
pushd build
cmake ..
cmake --build . --config Release --parallel
popd