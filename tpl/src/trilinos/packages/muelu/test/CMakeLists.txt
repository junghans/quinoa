ASSERT_DEFINED(
  ${PACKAGE_NAME}_ENABLE_Tpetra
  ${PACKAGE_NAME}_ENABLE_Ifpack2
  ${PACKAGE_NAME}_ENABLE_Amesos2
  )

TRIBITS_ADD_TEST_DIRECTORIES(unit_tests)

IF (${PACKAGE_NAME}_ENABLE_Kokkos_Refactor)
  TRIBITS_ADD_TEST_DIRECTORIES(unit_tests_kokkos)
  TRIBITS_ADD_TEST_DIRECTORIES(perf_tests_kokkos)
ENDIF()

IF(${PACKAGE_NAME}_ENABLE_BROKEN_TESTS)

# experimental data
IF (${PACKAGE_NAME}_ENABLE_Experimental)
  TRIBITS_ADD_TEST_DIRECTORIES(blockedtransfer)
ENDIF()

IF(Xpetra_INT_LONG_LONG AND HAVE_MUELU_INST_DOUBLE_INT_LONGLONGINT)
  TRIBITS_ADD_TEST_DIRECTORIES(longlong)
ENDIF()
TRIBITS_ADD_TEST_DIRECTORIES(factories)
TRIBITS_ADD_TEST_DIRECTORIES(maxwell)
TRIBITS_ADD_TEST_DIRECTORIES(navierstokes)
if (NOT MUELU_VALGRIND_TESTING)
  # JJH 4Feb2014:  This is failing the nightly MPI valgrind test.  It runs fine
  # interactively.  If/when I figure out the cause, I'll reenable this during
  # memory testing.
  TRIBITS_ADD_TEST_DIRECTORIES(paramlist)
ENDIF()
TRIBITS_ADD_TEST_DIRECTORIES(permutation)
TRIBITS_ADD_TEST_DIRECTORIES(profiling)
TRIBITS_ADD_TEST_DIRECTORIES(scaling)
TRIBITS_ADD_TEST_DIRECTORIES(simple1D)
TRIBITS_ADD_TEST_DIRECTORIES(viz)
TRIBITS_ADD_TEST_DIRECTORIES(toggletransfer)
TRIBITS_ADD_TEST_DIRECTORIES(perf_tests)

if (NOT MUELU_VALGRIND_TESTING)
  TRIBITS_ADD_TEST_DIRECTORIES(convergence)
  TRIBITS_ADD_TEST_DIRECTORIES(interface)
  IF (${PACKAGE_NAME}_ENABLE_Tpetra AND ${PACKAGE_NAME}_ENABLE_Ifpack2 AND ${PACKAGE_NAME}_ENABLE_Amesos2)
    TRIBITS_ADD_TEST_DIRECTORIES(helmholtz)
  ENDIF()

ELSE()
  MESSAGE(STATUS "Skipping EasyParameterListInterpreter and Helmholtz due to valgrind testing.")
ENDIF()

#TRIBITS_ADD_TEST_DIRECTORIES(mhdvanka)

ENDIF() # IF(${PACKAGE_NAME}_ENABLE_BROKEN_TESTS)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)

SET(SOURCES "")
