# Copyright (c) 2020 The Orbit Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

cmake_minimum_required(VERSION 3.15)

project(OrbitCore)
add_library(OrbitCore STATIC)

target_compile_options(OrbitCore PRIVATE ${STRICT_COMPILE_FLAGS})

target_sources(
  OrbitCore
  PUBLIC BlockChain.h
         Callstack.h
         CallstackTypes.h
         Capture.h
         CaptureData.h
         EventBuffer.h
         FunctionUtils.h
         Introspection.h
         LinuxCallstackEvent.h
         LinuxTracingBuffer.h
         OrbitModule.h
         OrbitProcess.h
         Params.h
         Path.h
         Pdb.h
         Profiling.h
         RingBuffer.h
         SamplingProfiler.h
         ScopeTimer.h
         StringManager.h
         SymbolHelper.h
         Threading.h
         TidAndThreadName.h
         Utils.h)

target_sources(
  OrbitCore
  PRIVATE Capture.cpp
          CaptureData.cpp
          EventBuffer.cpp
          FunctionUtils.cpp
          Introspection.cpp
          LinuxTracingBuffer.cpp
          OrbitModule.cpp
          OrbitProcess.cpp
          Params.cpp
          Path.cpp
          Pdb.cpp
          SamplingProfiler.cpp
          ScopeTimer.cpp
          StringManager.cpp
          SymbolHelper.cpp
          Utils.cpp)

if(NOT WIN32)
  target_sources(
    OrbitCore
    PUBLIC LinuxUtils.h)

  target_sources(
    OrbitCore
    PRIVATE LinuxUtils.cpp)
endif()

target_include_directories(OrbitCore PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  OrbitCore
  PUBLIC ElfUtils
         OrbitBase
         OrbitClientProtos
         OrbitProtos
         xxHash::xxHash
         concurrentqueue::concurrentqueue
         multicore::multicore
         oqpi::oqpi
         abseil::abseil
         llvm_object::llvm_object
         Outcome::Outcome)

if(WIN32)
  target_link_libraries(OrbitCore PRIVATE dbghelp.lib)
  target_compile_definitions(OrbitCore PUBLIC -D_WIN32_WINNT=0x0700)
  target_compile_definitions(OrbitCore PUBLIC -DNTDDI_VERSION=0x06030000)
endif()

target_compile_definitions(OrbitCore PUBLIC WIN32_LEAN_AND_MEAN)
target_compile_definitions(OrbitCore PUBLIC INITGUID)
target_compile_definitions(OrbitCore PUBLIC NOMINMAX)
target_compile_definitions(OrbitCore PUBLIC UNICODE)
target_compile_definitions(OrbitCore PUBLIC _UNICODE)

target_compile_features(OrbitCore PUBLIC cxx_std_11)

add_executable(OrbitCoreTests)

target_compile_options(OrbitCoreTests PRIVATE ${STRICT_COMPILE_FLAGS})

target_sources(OrbitCoreTests PRIVATE
    BlockChainTest.cpp
    LinuxTracingBufferTest.cpp
    PathTest.cpp
    RingBufferTest.cpp
    StringManagerTest.cpp
    SymbolHelperTest.cpp
    UtilsTest.cpp
)

if(NOT WIN32)
  target_sources(OrbitCoreTests PRIVATE OrbitModuleTest.cpp)
endif()

target_link_libraries(
  OrbitCoreTests
  PRIVATE OrbitCore
          GTest::Main
          llvm_object::llvm_object
          abseil::abseil)

register_test(OrbitCoreTests)

add_fuzzer(ModuleLoadSymbolsFuzzer ModuleLoadSymbolsFuzzer.cpp)
target_link_libraries(
  ModuleLoadSymbolsFuzzer PRIVATE OrbitCore
                                  libprotobuf-mutator::libprotobuf-mutator)

add_fuzzer(SymbolHelperLoadSymbolsCollectorFuzzer
           SymbolHelperLoadSymbolsCollectorFuzzer.cpp)
target_link_libraries(SymbolHelperLoadSymbolsCollectorFuzzer PRIVATE OrbitCore)
