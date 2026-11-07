# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: 2020 Danny Angelo Carminati Grein

function(add_cxx_option flag)
  if(ARGV1 STREQUAL "RELEASE_ONLY")
    set(should_add "$<CONFIG:Release>")
  else()
    set(should_add 1)
  endif()
  set(is_cxx "$<COMPILE_LANGUAGE:CXX>")
  add_compile_options("$<$<AND:${is_cxx},${should_add}>:${flag}>")
endfunction()

function(add_release_option flag)
  add_compile_options("$<$<CONFIG:Release>:${flag}>")
endfunction()

function(add_debug_option flag)
  add_compile_options("$<$<NOT:$<CONFIG:Release>>:${flag}>")
endfunction()

if(CMAKE_GENERATOR MATCHES "Ninja")
  add_compile_options(-fdiagnostics-color)
elseif(CMAKE_GENERATOR MATCHES "Visual Studio")
  add_compile_options("/MP")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  compile_definitions(_DEBUG DEBUG_ONLY)
  compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
  add_cxx_option(/EHsc)

  # Remove unreferenced inline functions/data to reduce link time and catch bugs
  add_compile_options(/Zc:inline)
  # Assume `new` (w/o std::nothrow) throws to reduce binary size
  add_cxx_option(/Zc:throwingNew)
  # Enforce strict volatile semantics as per ISO C++
  add_cxx_option(/volatile:iso)

  string(APPEND CMAKE_EXE_LINKER_FLAGS " /NXCOMPAT")
else()
  compile_definitions(_DEBUG DEBUG_ONLY)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fno-strict-aliasing)
    add_compile_options(-Wswitch-default)
    add_compile_options(-Wmissing-variable-declarations)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wno-switch-default) # SDL_stdinc.h
    add_compile_options(-Wlogical-op)
    if(WIN32)
      # MinGW CRT printf lacks %zu: route through the ANSI stdio layer so
      # size_t conversions are understood (safe with static linking)
      compile_definitions(__USE_MINGW_ANSI_STDIO=1)
    endif()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86)$" AND NOT XUO_EMSC)
      # soloud (external/ass.h) enables its SSE intrinsics path on any x86;
      # the i386 baseline compiles none of it without explicit -msse
      add_compile_options(-msse)
    endif()
  endif()

  # these are the only we want to really always disable
  add_compile_options(-Wno-sign-compare)
  add_compile_options(-Wno-unused-parameter)
  add_compile_options(-Wno-missing-field-initializers)
  add_compile_options(-Wno-float-equal)
  #add_compile_options(-Wconversion)
  #add_compile_options(-Wzero-as-null-pointer-constant)

  add_compile_options(-Wtype-limits)
  add_compile_options(-Wignored-qualifiers)
  add_compile_options(-Wuninitialized)
  add_compile_options(-Wshadow)
  add_compile_options(-Winit-self)

  #add_cxx_option(-fvisibility-inlines-hidden)
  #add_cxx_option(-fvisibility=hidden)
  #add_cxx_option(-fpermissive) # FIXME GCC
  add_cxx_option(-fno-rtti)
  add_cxx_option(-fno-exceptions)
  add_cxx_option(-fomit-frame-pointer RELEASE_ONLY)
  add_debug_option(-g)

  add_compile_options(-Wno-unknown-pragmas)
endif()

# Compiler extensions

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  # enable the latest C++ standard feature set,
  # and also disable MSVC specific extensions
  # to be even more standards compliant.
  add_cxx_option(/std:c++latest)
else()
  #add_compile_options(-pthread)
  set(THREAD "-pthread")
  if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(LOADER "-ldl")
  endif()
endif()

if(ENABLE_LTO)
  # LTO is applied through the per-configuration flag variables below (the same
  # variables the cache stores for Release/MInSizeRel) so it reaches the client
  # and every static archive linked into it: xuocore, common, external,
  # external/gfx and the assistant are compiled to bitcode and optimized
  # together at the final link. Only the optimized build types get it - Debug
  # is meant to stay quick - and -DENABLE_LTO=OFF removes it again (the cache
  # entry is what a previously configured tree keeps).
  # Emscripten links through wasm-ld, which has no LTO support.
  if(XUO_EMSC)
    message(STATUS "LTO: disabled (emscripten)")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # MSVC: /GL emits the bitcode, /LTCG optimizes it at link time
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GL")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /GL")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /GL")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /LTCG")
    message(STATUS "LTO: enabled for Release/MinSizeRel (/GL + /LTCG)")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # thin LTO: incremental-friendly, and the spelling apple clang and
    # upstream clang both understand (lld/gold/bfd all read the bitcode)
    set(_xuo_lto_flag "-flto=thin")
  else()
    set(_xuo_lto_flag "-flto")
    if(NOT CMAKE_AR)
      # plain ar cannot read LTO objects; binutils ships the wrappers
      set(CMAKE_AR gcc-ar)
      set(CMAKE_RANLIB gcc-ranlib)
    endif()
  endif()
  if(_xuo_lto_flag)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${_xuo_lto_flag}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${_xuo_lto_flag}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${_xuo_lto_flag}")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${_xuo_lto_flag}")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${_xuo_lto_flag}")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} ${_xuo_lto_flag}")
    message(STATUS "LTO: enabled for Release/MinSizeRel (${_xuo_lto_flag})")
  endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-DXUO_WINDOWS=1)
  set(XUO_WINDOWS 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
  add_definitions(-DXUO_EMSC=1)
  set(XUO_EMSC 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_definitions(-DXUO_LINUX=1)
  set(XUO_LINUX 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  add_definitions(-DXUO_OSX=1)
  set(XUO_OSX 1)
endif()
