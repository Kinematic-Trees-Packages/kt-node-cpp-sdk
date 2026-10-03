#!/usr/bin/env bash
set -euo pipefail
out="${KTM_BUILD_OUTPUT:-build/ktm-output}"
rm -rf "$out"
mkdir -p "$out/compiled/lib" "$out/compiled/include" "$out/source"
cp -a package.ktm.json runtime.json "$out/"
cmake -S . -B build/pack -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build/pack --target {{KTM_CREATE_MODULE_NAME}}
find build/pack -maxdepth 3 -type f \( -name 'lib{{KTM_CREATE_MODULE_NAME}}*.so' -o -name 'lib{{KTM_CREATE_MODULE_NAME}}*.a' \) -exec cp -a {} "$out/compiled/lib/" \;
cp -a include/. "$out/compiled/include/"
cp -a README.md scripts CMakeLists.txt include src examples tests "$out/source"/
echo "Built {{KTM_CREATE_PROJECT_NAME}} C++ compiled package into $out/compiled"
