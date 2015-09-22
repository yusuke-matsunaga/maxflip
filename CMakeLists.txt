# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


# C++11 の指定
set (CMAKE_CXX_STANDARD 11)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (maxflip)
set (MAXFLIP_VERSION_MAJOR 1)
set (MAXFLIP_VERSION_MINOR 0)
set (MAXFLIP_VERSION ${MAXFLIP_VERSION_MAJOR}.${MAXFLIP_VERSION_MINOR})


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )

#include (FindPythonInterp)
#include (FindPythonLibs)

find_package(YmTools REQUIRED)

find_package(Gperftools)

#include (UseCython)
include (ym_add_library)


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
include ( ym_use_gtest )
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)

enable_testing()


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${YmTools_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#if (CYTHON_FOUND)
#  add_subdirectory (py-satpg)
#endif (CYTHON_FOUND)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (maxflip_SOURCES
  src/maxflip.cc
  src/pure_mc.cc
  src/sa.cc
  src/count.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
add_executable ( maxflip
  ${maxflip_SOURCES}
  )

target_compile_options ( maxflip
  PRIVATE "-O3"
  )

target_compile_definitions ( maxflip
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( maxflip
  ym_networks
  )

add_executable ( maxflip_p
  ${maxflip_SOURCES}
  )

target_compile_options ( maxflip_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions ( maxflip_p
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( maxflip_p
  ym_networks_p
  )

add_executable ( maxflip_d
  ${maxflip_SOURCES}
  )

target_compile_options ( maxflip_d
  PRIVATE "-g"
  )

target_link_libraries ( maxflip_d
  ym_networks_d
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
