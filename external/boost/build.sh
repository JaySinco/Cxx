# need to pass -DBoost_COMPILER="-vc141" to CMake to make it work

./b2.exe --with-system --with-thread --with-filesystem --with-chrono\
    --with-program_options --with-test\
    toolset=msvc variant=release link=static \
    architecture=x86 address-model=64 threading=multi \
    runtime-link=shared