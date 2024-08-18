@echo off

for %%A in (%*) do (
    if "%%A"=="clean" (
        echo Performing clean rebuild...
        rmdir /s /q build
        goto configure
    )
)

:configure

for %%A in (%*) do (
    if "%%A"=="clang" (
        echo Configuring for Clang build...
        cmake -B build . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G "Ninja Multi-Config"
        goto build
    ) else if "%%A"=="web" (
        echo Configuring for WebAssembly build...
        call emcmake cmake -B build . -G "Ninja Multi-Config"
        goto build
    )
)

echo Configuring for default build...
cmake -B build . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

:build

echo Building project...
cmake --build build --config Release --parallel
