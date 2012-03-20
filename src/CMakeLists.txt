add_executable(emulator VM.cxx main.cpp)

set_target_properties(emulator PROPERTIES COMPILE_FLAGS "-std=c++0x")

