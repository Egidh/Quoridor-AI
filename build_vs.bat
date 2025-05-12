rem Use this batch file to build for Visual Studio 17
rmdir /s /q _build_vs
mkdir _build_vs
cd _build_vs
cmake .. -G "Visual Studio 17 2022" -A x64
