cmake_minimum_required(VERSION 3.16)
project(KTNodeCppSDK LANGUAGES CXX)

find_package(KTMessages 0.1.0 CONFIG REQUIRED)

add_library(KTNodeCppSDK INTERFACE)
add_library(KTNode::CppSDK ALIAS KTNodeCppSDK)
target_compile_features(KTNodeCppSDK INTERFACE cxx_std_17)
target_include_directories(KTNodeCppSDK INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(KTNodeCppSDK INTERFACE KTMessages::KTMessages)

include(CTest)
if(BUILD_TESTING)
  add_executable(ktnode_cpp_vision_test tests/vision_test.cpp)
  target_link_libraries(ktnode_cpp_vision_test PRIVATE KTNode::CppSDK)
  add_test(NAME ktnode_cpp_vision_test COMMAND ktnode_cpp_vision_test)
endif()
