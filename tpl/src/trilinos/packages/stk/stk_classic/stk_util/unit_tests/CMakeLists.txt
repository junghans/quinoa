
INCLUDE(TribitsAddExecutableAndTest)

ASSERT_DEFINED(PACKAGE_SOURCE_DIR CMAKE_CURRENT_SOURCE_DIR)

if ( TPL_ENABLE_MPI )
  ADD_SUBDIRECTORY(parallel)
ENDIF()

TRIBITS_CONFIGURE_FILE(${PACKAGE_NAME}_config.h)

FILE(GLOB SOURCES diag/*.cpp environment/*.cpp util/*.cpp)

APPEND_SET( SOURCES
 UnitTestBoost.cpp
 UnitTestMain.cpp
)

if ( TPL_ENABLE_MPI )
  APPEND_SET( SOURCES
    UnitTestParallel.cpp
  )
ENDIF()

TRIBITS_ADD_EXECUTABLE_AND_TEST(
    util_UnitTest
    SOURCES ${SOURCES}
    DEPLIBS stkclassic_util_parallel stkclassic_util_diag
    COMM serial mpi
    NUM_MPI_PROCS 1
    STANDARD_PASS_OUTPUT
    )


