# Third-party libraries paths

#### MKL (optional)
find_library(MKL_INTERFACE_LIBRARY
             NAMES mkl_intel_ilp64
             PATHS $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

find_library(MKL_SEQUENTIAL_LAYER_LIBRARY
             NAMES mkl_sequential
             PATHS $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

#find_library(MKL_THREADED_LAYER_LIBRARY
#             NAMES mkl_intel_thread
#             PATHS $ENV{MKLROOT}/lib/intel64
#                   $ENV{INTEL}/mkl/lib/intel64
#             NO_DEFAULT_PATH)

find_library(MKL_CORE_LIBRARY
             NAMES mkl_core
             PATHS $ENV{MKLROOT}/lib/intel64
                   $ENV{INTEL}/mkl/lib/intel64
             NO_DEFAULT_PATH)

find_path(MKL_INCLUDE_PATH mkl.h
          $ENV{MKLROOT}/include
          $ENV{INTEL}/mkl/include
          NO_DEFAULT_PATH)

if (MKL_INTERFACE_LIBRARY AND
    MKL_SEQUENTIAL_LAYER_LIBRARY AND
    #MKL_THREADED_LAYER_LIBRARY AND
    MKL_CORE_LIBRARY)
  message(STATUS "Found MKL:")
  message(STATUS " * MKL_INTERFACE_LIBRARY: ${MKL_INTERFACE_LIBRARY}")
  message(STATUS " * MKL_SEQUENTIAL_LAYER_LIBRARY: ${MKL_SEQUENTIAL_LAYER_LIBRARY}")
  #message(STATUS " * MKL_THREADED_LAYER_LIBRARY: ${MKL_THREADED_LAYER_LIBRARY}")
  message(STATUS " * MKL_CORE_LIBRARY: ${MKL_CORE_LIBRARY}")
  set(HAS_MKL on)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMKL_ILP64 -m64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64 -m64")
else()
  message(STATUS "MKL NOT found: Intel MKL VSL RNGs will not be available")
  set(MKL_INTERFACE_LIBRARY "")
  set(MKL_SEQUENTIAL_LAYER_LIBRARY "")
  #set(MKL_THREADED_LAYER_LIBRARY "")
  set(MKL_CORE_LIBRARY "")
  set(HAS_MKL off)
endif()

#### Boost C++ libraries
if (NOT NO_SYSTEM_BOOST)
  set(BOOST_INCLUDEDIR ${TPL_DIR}/include) # prefer ours
  find_package(Boost REQUIRED)
endif()
if(Boost_FOUND)
  #message(STATUS "Boost at ${Boost_INCLUDE_DIR} (include)")
  include_directories(${Boost_INCLUDE_DIR})
endif()

#### NetCDF library
if (NOT NO_SYSTEM_NETCDF)
  set(NETCDF_ROOT ${TPL_DIR}) # prefer ours
  find_package(NetCDF REQUIRED)
endif()
if(NETCDF_FOUND)
  #message(STATUS "NetCDF at ${NETCDF_INCLUDES} (include) and at ${NETCDF_LIBRARIES} (lib)")
endif()

#### ExodusII/Nemesis library
if (NOT NO_SYSTEM_EXODUS)
  set(EXODUS_ROOT ${TPL_DIR}) # prefer ours
  find_package(Exodus REQUIRED)
endif()
if(EXODUS_FOUND)
  #message(STATUS "ExodusII/Nemesis at ${EXODUS_INCLUDES} (exodus include), ${NEMESIS_INCLUDES} (nemesis include) and at ${EXODUS_LIBRARIES} (exodus lib), ${NEMESIS_LIBRARIES} (nemesis lib)")
endif()

#### Zoltan library
if (NOT NO_SYSTEM_ZOLTAN)
  set(ZOLTAN_ROOT ${TPL_DIR}) # prefer ours
  find_package(Zoltan REQUIRED)
endif()
if(ZOLTAN_FOUND)
  #message(STATUS "Zoltan found at ${ZOLTAN_INCLUDES} (include) and at ${ZOLTAN_LIBRARIES} (lib)")
endif()

#### Hypre library
if (NOT NO_SYSTEM_HYPRE)
  set(HYPRE_ROOT ${TPL_DIR}) # prefer ours
  find_package(Hypre REQUIRED)
endif()
if(HYPRE_FOUND)
  #message(STATUS "Hypre found at ${HYPRE_INCLUDES} (include) and at ${HYPRE_LIBRARIES} (lib)")
endif()

#### BLAS/LAPACK library
if (HAS_MKL)    # prefer Intel's MKL's BLAS/LAPACK if MKL is available
  message(STATUS "Found BLAS/LAPACK: ${MKL_INTERFACE_LIBRARY};${MKL_CORE_LIBRARY};${MKL_SEQUENTIAL_LAYER_LIBRARY} using via MKL's C-interface")
else()
  find_package(LAPACK REQUIRED)
  if(LAPACK_FOUND)
    # find C-interface
    find_path(LAPACKE_PATH lapacke.h DOC "C-interface to LAPACK")
    find_library(LAPACKE_LIB NAMES lapacke REQUIRED)
    message(STATUS "Found BLAS/LAPACK: ${LAPACK_LIBRARIES} using via C-interface ${LAPACKE_PATH}/lapacke.h and ${LAPACKE_LIB}")
  endif()
endif()

#### PStreams library
if (NOT NO_SYSTEM_PSTREAMS)
  set(PSTREAMS_ROOT ${TPL_DIR}) # prefer ours
  find_package(PStreams REQUIRED)
endif()
if(PSTREAMS_FOUND)
  #message(STATUS "PStreams at ${PSTREAMS_INCLUDES}")
endif()

#### RNGSSE2 library
set(RNGSSE_LIBRARY "NOTFOUND")
find_library(RNGSSE_LIBRARY
             NAMES rngsse
             PATHS ${TPL_DIR}/lib
             NO_DEFAULT_PATH
             REQUIRED)

#### TestU01 library
set(TESTU01_LIBRARY "NOTFOUND")
find_library(TESTU01_LIBRARY
             NAMES testu01
             PATHS ${TPL_DIR}/lib
             NO_DEFAULT_PATH
             REQUIRED)
set(TESTU01_PROBDIST_LIBRARY "NOTFOUND")
find_library(TESTU01_PROBDIST_LIBRARY
             NAMES probdist
             PATHS ${TPL_DIR}/lib
             NO_DEFAULT_PATH
             REQUIRED)
set(TESTU01_MYLIB_LIBRARY "NOTFOUND")
find_library(TESTU01_MYLIB_LIBRARY
             NAMES mylib
             PATHS ${TPL_DIR}/lib
             NO_DEFAULT_PATH
             REQUIRED)

##### Silo
#set(SILO_LIBRARY "NOTFOUND")
#find_library(SILO_LIBRARY
#             NAMES siloh5
#             PATHS ${TPL_DIR}/lib
#             NO_DEFAULT_PATH
#             REQUIRED)
