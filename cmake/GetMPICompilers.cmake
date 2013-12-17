if(__get_mpi_compilers)
	return()
endif()
set(__get_mpi_compilers YES)


function(get_mpi_compilers)

  # Find MPI
  find_package(MPI REQUIRED)

  # Find out underlying C compiler
  execute_process(
    COMMAND           ${MPI_C_COMPILER} "-show"
    OUTPUT_VARIABLE   UNDERLYING_C_COMPILER
  )
  if (UNDERLYING_C_COMPILER)
    string(REPLACE " " ";" C_COMP ${UNDERLYING_C_COMPILER})
    LIST(GET C_COMP 0 UNDERLYING_C_COMPILER)
    string(REGEX REPLACE "[\r\n]" "" UNDERLYING_C_COMPILER
           "${UNDERLYING_C_COMPILER}")
    set(UNDERLYING_C_COMPILER "${UNDERLYING_C_COMPILER}" PARENT_SCOPE)
  endif()

  # Find out underlying C++ compiler
  execute_process(
    COMMAND           ${MPI_CXX_COMPILER} "-show"
    OUTPUT_VARIABLE   UNDERLYING_CXX_COMPILER
  )
  if (UNDERLYING_CXX_COMPILER)
    string(REPLACE " " ";" CXX_COMP ${UNDERLYING_CXX_COMPILER})
    LIST(GET CXX_COMP 0 UNDERLYING_CXX_COMPILER)
    string(REGEX REPLACE "[\r\n]" "" UNDERLYING_CXX_COMPILER
           "${UNDERLYING_CXX_COMPILER}")
    set(UNDERLYING_CXX_COMPILER "${UNDERLYING_CXX_COMPILER}" PARENT_SCOPE)
  endif()

  # Find out underlying Fortran compiler
  execute_process(
    COMMAND           ${MPI_Fortran_COMPILER} "-show"
    OUTPUT_VARIABLE   UNDERLYING_FORTRAN_COMPILER
  )
  if (UNDERLYING_FORTRAN_COMPILER)
    string(REPLACE " " ";" F_COMP ${UNDERLYING_FORTRAN_COMPILER})
    LIST(GET F_COMP 0 UNDERLYING_FORTRAN_COMPILER)
    string(REGEX REPLACE "[\r\n]" "" UNDERLYING_FORTRAN_COMPILER
           "${UNDERLYING_FORTRAN_COMPILER}")
    set(UNDERLYING_FORTRAN_COMPILER "${UNDERLYING_FORTRAN_COMPILER}" PARENT_SCOPE)
  endif()

  # Echo compilers
  if (MPI_C_COMPILER)
    MESSAGE(STATUS "MPI C wrapper: " ${MPI_C_COMPILER})
  endif()

  if (MPI_CXX_COMPILER)
    MESSAGE(STATUS "MPI C++ wrapper: " ${MPI_CXX_COMPILER})
  endif()

  if (MPI_Fortran_COMPILER)
    MESSAGE(STATUS "MPI Fortran wrapper: " ${MPI_Fortran_COMPILER})
  endif()

  if (UNDERLYING_C_COMPILER)
    MESSAGE(STATUS "Underlying C compiler: " ${UNDERLYING_C_COMPILER})
  endif()

  if (UNDERLYING_CXX_COMPILER)
    MESSAGE(STATUS "Underlying C++ compiler: " ${UNDERLYING_CXX_COMPILER})
  endif()

  if (UNDERLYING_FORTRAN_COMPILER)
    MESSAGE(STATUS "Underlying Fortran compiler: "
            ${UNDERLYING_FORTRAN_COMPILER})
  endif()

endfunction()
