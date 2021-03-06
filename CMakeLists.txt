cmake_minimum_required(VERSION 3.1.0)

project(chuffed CXX C)
# The version number.
set(chuffed_VERSION_MAJOR 0)
set(chuffed_VERSION_MINOR 10)
set(chuffed_VERSION_PATCH 3)

### Additional Definitions:
option(STATIC "compile with the -static linker flag" OFF)

# ------------- Compiler Configuration -------------

if(STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug"
    "Release"
    "MinSizeRel"
    "RelWithDebInfo"
  )
endif()

if(APPLE)
  execute_process(COMMAND xcrun --show-sdk-path OUTPUT_VARIABLE OSX_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_OSX_SYSROOT ${OSX_SYSROOT})
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")
endif(APPLE)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

include_directories("chuffed")

# ------------- CP Profiler Integration -------------

if(EXISTS "${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf")
  include_directories("submodules/cp-profiler-integration/protobuf")
  if(WIN32)
    include_directories("submodules/cp-profiler-integration/protobuf/config_win")
  else()
    include_directories("submodules/cp-profiler-integration/protobuf/config_unix")
  endif()
else()
  message("Configuring without CP Profiler support.")
endif()

include_directories(".")
include_directories(${CMAKE_BINARY_DIR})
if(WIN32)
  add_definitions(-DYY_NO_UNISTD_H)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf")

  set(CHUFFED_PROFILER_SOURCES
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/connector.cpp
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/message.pb.cpp

    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/compiler/importer.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/compiler/parser.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/descriptor.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/descriptor.pb.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/descriptor_database.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/dynamic_message.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/extension_set.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/extension_set_heavy.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/generated_message_reflection.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/generated_message_util.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/coded_stream.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/gzip_stream.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/printer.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/strtod.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/tokenizer.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/zero_copy_stream.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/zero_copy_stream_impl.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/io/zero_copy_stream_impl_lite.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/message.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/message_lite.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/reflection_ops.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/repeated_field.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/service.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/atomicops_internals_x86_gcc.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/common.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/once.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/stringprintf.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/structurally_valid.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/strutil.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/stubs/substitute.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/text_format.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/unknown_field_set.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/wire_format.cc
    ${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf/google/protobuf/wire_format_lite.cc
  )

endif()

# ------------- Main Chuffed Definition -------------

add_library(chuffed
  chuffed/vars/int-var.cpp
  chuffed/vars/int-var-el.cpp
  chuffed/vars/modelling.cpp
  chuffed/vars/int-var-sl.cpp
  chuffed/vars/bool-view.cpp
  chuffed/vars/int-var-ll.cpp
  chuffed/parallel/master.cpp
  chuffed/parallel/slave.cpp
  chuffed/ldsb/ldsb.cpp
  chuffed/globals/subcircuit.cpp
  chuffed/globals/mddglobals.cpp
  chuffed/globals/sym-break.cpp
  chuffed/globals/linear-bool.cpp
  chuffed/globals/linear-bool-decomp.cpp
  chuffed/globals/well-founded.cpp
  chuffed/globals/circuit.cpp
  chuffed/globals/minimum.cpp
  chuffed/globals/alldiff.cpp
  chuffed/globals/template.cpp
  chuffed/globals/directives.cpp
  chuffed/globals/cumulative.cpp
  chuffed/globals/cumulativeCalendar.cpp
  chuffed/globals/disjunctive.cpp
  chuffed/globals/regular.cpp
  chuffed/globals/lex.cpp
  chuffed/globals/table.cpp
  chuffed/mdd/MDD.cpp
  chuffed/mdd/mdd_prop.cpp
  chuffed/mdd/mdd_to_lgraph.cpp
  chuffed/mdd/MurmurHash3.cpp
  chuffed/mdd/opcache.cpp
  chuffed/mdd/weighted_dfa.cpp
  chuffed/mdd/wmdd_prop.cpp
  chuffed/mip/mip.cpp
  chuffed/mip/recalc.cpp
  chuffed/mip/simplex.cpp
  chuffed/primitives/element.cpp
  chuffed/primitives/bool.cpp
  chuffed/primitives/linear.cpp
  chuffed/primitives/arithmetic.cpp
  chuffed/primitives/domain.cpp
  chuffed/primitives/binary.cpp
  chuffed/branching/branching.cpp
  chuffed/core/init.cpp
  chuffed/core/stats.cpp
  chuffed/core/engine.cpp
  chuffed/core/options.cpp
  chuffed/core/sat.cpp
  chuffed/core/conflict.cpp
  chuffed/vars/int-var.h
  chuffed/vars/vars.h
  chuffed/vars/int-var-sl.h
  chuffed/vars/int-var-ll.h
  chuffed/vars/int-view.h
  chuffed/vars/int-var-el.h
  chuffed/vars/modelling.h
  chuffed/vars/bool-view.h
  chuffed/parallel/parallel.h
  chuffed/support/BVec.h
  chuffed/support/ParseUtils.h
  chuffed/support/vec.h
  chuffed/support/misc.h
  chuffed/support/heap.h
  chuffed/support/sparse_set.h
  chuffed/ldsb/ldsb.h
  chuffed/globals/globals.h
  chuffed/globals/mddglobals.h
  chuffed/mdd/CFG.h
  chuffed/mdd/circ_fns.h
  chuffed/mdd/circutil.h
  chuffed/mdd/CYK.h
  chuffed/mdd/MDD.h
  chuffed/mdd/mdd_prop.h
  chuffed/mdd/mdd_to_lgraph.h
  chuffed/mdd/MurmurHash3.h
  chuffed/mdd/opcache.h
  chuffed/mdd/opts.h
  chuffed/mdd/sorters.h
  chuffed/mdd/weighted_dfa.h
  chuffed/mdd/wmdd_prop.h
  chuffed/mip/mip.h
  chuffed/mip/simplex.h
  chuffed/primitives/primitives.h
  chuffed/branching/branching.h
  chuffed/core/sat.h
  chuffed/core/engine.h
  chuffed/core/sat-types.h
  chuffed/core/options.h
  chuffed/core/propagator.h
  ${CHUFFED_PROFILER_SOURCES}
)

if(EXISTS "${CMAKE_SOURCE_DIR}/submodules/cp-profiler-integration/protobuf")
  target_compile_definitions(chuffed PRIVATE HAS_PROFILER)
endif()

# ------------- FZN Chuffed -------------

find_package(BISON)
if(BISON_FOUND)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/chuffed/flatzinc)
  bison_target(FZNParser
    ${PROJECT_SOURCE_DIR}/chuffed/flatzinc/parser.yxx
    ${PROJECT_BINARY_DIR}/parser.tab.cpp
    DEFINES_FILE ${PROJECT_BINARY_DIR}/chuffed/flatzinc/parser.tab.h
    COMPILE_FLAGS "-l"
  )
else()
  message(WARNING "Bison cannot be run. Using cached file, which may be out of date.")
  set(BISON_FZNParser_OUTPUTS
    ${PROJECT_SOURCE_DIR}/chuffed/flatzinc/parser.tab.cpp
    ${PROJECT_SOURCE_DIR}/chuffed/flatzinc/parser.tab.h
  )
endif()

find_package(FLEX)
if(FLEX_FOUND)
  flex_target(FZNLexer
    ${PROJECT_SOURCE_DIR}/chuffed/flatzinc/lexer.lxx
    ${PROJECT_BINARY_DIR}/lexer.yy.cpp
    COMPILE_FLAGS "-L"
  )
  add_flex_bison_dependency(FZNLexer FZNParser)
else()
  message(WARNING "Flex cannot be run. Using cached file, which may be out of date.")
  set(FLEX_FZNLexer_OUTPUTS ${PROJECT_SOURCE_DIR}/chuffed/flatzinc/lexer.yy.cpp)
endif()

add_library(chuffed_fzn
  ${parser_cpp}
  chuffed/flatzinc/registry.cpp
  chuffed/flatzinc/flatzinc.cpp
  ${FLEX_FZNLexer_OUTPUTS}
  ${BISON_FZNParser_OUTPUTS}
  chuffed/flatzinc/flatzinc.h
  chuffed/flatzinc/ast.h
)

add_executable(fzn-chuffed chuffed/flatzinc/fzn-chuffed.cpp)
target_link_libraries(fzn-chuffed chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

# ------------- Chuffed Examples -------------

add_executable(bibd EXCLUDE_FROM_ALL chuffed/examples/bibd.cpp)
target_link_libraries(bibd chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(blackhole EXCLUDE_FROM_ALL chuffed/examples/blackhole.cpp)
target_link_libraries(blackhole chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(concert_hall EXCLUDE_FROM_ALL chuffed/examples/concert_hall.cpp)
target_link_libraries(concert_hall chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(concert_hall_sym EXCLUDE_FROM_ALL chuffed/examples/concert_hall_sym.cpp)
target_link_libraries(concert_hall_sym chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(golomb EXCLUDE_FROM_ALL chuffed/examples/golomb.cpp)
target_link_libraries(golomb chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(graceful_graph EXCLUDE_FROM_ALL chuffed/examples/graceful_graph.cpp)
target_link_libraries(graceful_graph chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(graph_colouring EXCLUDE_FROM_ALL chuffed/examples/graph_colouring.cpp)
target_link_libraries(graph_colouring chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(graph_colouring_sym EXCLUDE_FROM_ALL chuffed/examples/graph_colouring_sym.cpp)
target_link_libraries(graph_colouring_sym chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(graph_colouring_sym2 EXCLUDE_FROM_ALL chuffed/examples/graph_colouring_sym2.cpp)
target_link_libraries(graph_colouring_sym2 chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(grid_colouring EXCLUDE_FROM_ALL chuffed/examples/grid_colouring.cpp)
target_link_libraries(grid_colouring chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(latin_square EXCLUDE_FROM_ALL chuffed/examples/latin_square.cpp)
target_link_libraries(latin_square chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(magic_square EXCLUDE_FROM_ALL chuffed/examples/magic_square.cpp)
target_link_libraries(magic_square chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(mosp EXCLUDE_FROM_ALL chuffed/examples/mosp.cpp)
target_link_libraries(mosp chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(nn_queens EXCLUDE_FROM_ALL chuffed/examples/nn_queens.cpp)
target_link_libraries(nn_queens chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(non EXCLUDE_FROM_ALL chuffed/examples/non.cpp)
target_link_libraries(non chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(photo EXCLUDE_FROM_ALL chuffed/examples/photo.cpp)
target_link_libraries(photo chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(queens EXCLUDE_FROM_ALL chuffed/examples/queens.cpp)
target_link_libraries(queens chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(rcpsp EXCLUDE_FROM_ALL chuffed/examples/rcpsp.cpp)
target_link_libraries(rcpsp chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(shift EXCLUDE_FROM_ALL chuffed/examples/shift.cpp)
target_link_libraries(shift chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(steel_mill EXCLUDE_FROM_ALL chuffed/examples/steel_mill.cpp)
target_link_libraries(steel_mill chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

add_executable(wreg_shift EXCLUDE_FROM_ALL chuffed/examples/wreg_shift.cpp)
target_link_libraries(wreg_shift chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT})

find_package(ZLIB)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})

  add_executable(cross EXCLUDE_FROM_ALL chuffed/examples/cross.cpp)
  target_link_libraries(cross chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES})

  add_executable(fdpent EXCLUDE_FROM_ALL chuffed/examples/fdpent.cpp)
  target_link_libraries(fdpent chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES})

  add_executable(nurse EXCLUDE_FROM_ALL chuffed/examples/nurse.cpp)
  target_link_libraries(nurse chuffed_fzn chuffed ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES})

  set(ZLIB_EXAMPLES
    cross
    fdpent
    nurse
  )
endif()

add_custom_target(examples)
add_dependencies(examples
  bibd
  blackhole
  concert_hall
  concert_hall_sym
  golomb
  graceful_graph
  graph_colouring
  graph_colouring_sym
  graph_colouring_sym2
  grid_colouring
  latin_square
  magic_square
  mosp
  nn_queens
  non
  photo
  queens
  rcpsp
  shift
  steel_mill
  wreg_shift
  ${ZLIB_EXAMPLES}
)

# ------------- TARGET install -------------

configure_file(chuffed.msc.in chuffed.msc)
install(TARGETS fzn-chuffed RUNTIME DESTINATION bin)
install(DIRECTORY chuffed/flatzinc/mznlib DESTINATION share/chuffed)
install(FILES ${CMAKE_BINARY_DIR}/chuffed.msc DESTINATION .)
