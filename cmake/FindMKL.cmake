################################################################################
#
# \file      cmake/FindLMKL.cmake
# \author    J. Bakosi
# \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
# \brief     Find the Math Kernel Library from Intel
# \date      Wed 01 Jun 2016 01:29:27 PM MDT
#
################################################################################

# Find the Math Kernel Library from Intel
#
#  MKL_FOUND - System has MKL
#  MKL_INTERFACE_LIBRARY - MKL interface library
#  MKL_SEQUENTIAL_LAYER_LIBRARY - MKL sequential layer library
#  MKL_THREADED_LAYER_LIBRARY - MKL threaded layer library
#  MKL_CORE_LIBRARY - MKL core library
#
#  The environment variables MKLROOT and INTEL are used to find the library.
#  Everything else is ignored. If MKL is found "-DMKL_ILP64 -m64" is added to
#  CMAKE_C_FLAGS and CMAKE_CXX_FLAGS.
#
#  Example usage:
#
#  find_package(MKL)
#  if(MKL_FOUND)
#    target_link_libraries(TARGET
#                          ${MKL_INTERFACE_LIBRARY}
#                          ${MKL_CORE_LIBRARY}
#                          ${MKL_SEQUENTIAL_LAYER_LIBRARY})
#  endif()

# If already in cache, be silent
if (MKL_INTERFACE_LIBRARY AND
    MKL_SEQUENTIAL_LAYER_LIBRARY AND
    MKL_THREADED_LAYER_LIBRARY AND
    MKL_CORE_LIBRARY)
  set (MKL_FIND_QUIETLY TRUE)
endif()

if(NOT BUILD_SHARED_LIBS)
  set(INT_LIB "libmkl_intel_ilp64.a")
  set(SEQ_LIB "libmkl_sequential.a")
  set(THR_LIB "libmkl_intel_thread.a")
  set(COR_LIB "libmkl_core.a")
else()
  set(INT_LIB "mkl_intel_ilp64")
  set(SEQ_LIB "mkl_sequential")
  set(THR_LIB "mkl_intel_thread")
  set(COR_LIB "mkl_core")
endif()

find_library(MKL_INTERFACE_LIBRARY
             NAMES ${INT_LIB}
             PATHS $ENV{MKLROOT}/lib
                   $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

find_library(MKL_SEQUENTIAL_LAYER_LIBRARY
             NAMES ${SEQ_LIB}
             PATHS $ENV{MKLROOT}/lib
                   $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

find_library(MKL_THREADED_LAYER_LIBRARY
             NAMES ${THR_LIB}
             PATHS $ENV{MKLROOT}/lib
                   $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

find_library(MKL_CORE_LIBRARY
             NAMES ${COR_LIB}
             PATHS $ENV{MKLROOT}/lib
                   $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

if (MKL_INTERFACE_LIBRARY AND
    MKL_SEQUENTIAL_LAYER_LIBRARY AND
    MKL_THREADED_LAYER_LIBRARY AND
    MKL_CORE_LIBRARY)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMKL_ILP64 -m64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64 -m64")
else()
  set(MKL_INTERFACE_LIBRARY "")
  set(MKL_SEQUENTIAL_LAYER_LIBRARY "")
  set(MKL_THREADED_LAYER_LIBRARY "")
  set(MKL_CORE_LIBRARY "")
endif()

set(MKL_LIBS ${MKL_INTERFACE_LIBRARY} ${MKL_SEQUENTIAL_LAYER_LIBRARY}
             ${MKL_THREADED_LAYER_LIBRARY} ${MKL_CORE_LIBRARY})

# Handle the QUIETLY and REQUIRED arguments and set MKL_FOUND to TRUE if
# all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBS)

MARK_AS_ADVANCED(MKL_INTERFACE_LIBRARY
                 MKL_SEQUENTIAL_LAYER_LIBRARY
                 MKL_THREADED_LAYER_LIBRARY
                 MKL_CORE_LIBRARY
                 MKL_LIBS)
