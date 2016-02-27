# Find libc++.
# See libc++: http://libcxx.llvm.org, libc++abi: http://libcxxabi.llvm.org.
#
#  libc++_FOUND - System has libc++
#  libc++_INCLUDES - The libc++ include directory
#  libc++_LIBRARIES - The libraries needed to use libc++
#  libc++abi_LIBRARIES - The libraries needed to use libc++abi
#
#  Set libc++_ROOT before calling find_package to a path to add an additional
#  search path, e.g.,
#
#  Usage:
#
#  set(libc++_ROOT "/path/to/custom/libc++") # prefer over system
#  find_package(libc++)
#  if(libc++_FOUND)
#    target_link_libraries (TARGET ${libc++_LIBRARIES} ${libc++abi_LIBRARIES})
#  endif()

if(libc++_INCLUDES AND libc++_LIBRARIES AND libc++abi_LIBRARIES)
  # Already in cache, be silent
  set (libc++_FIND_QUIETLY TRUE)
endif()

FIND_PATH(libc++_INCLUDES NAMES cxxabi.h HINTS ${libc++_ROOT}/include
                                               /usr/include/c++/v1)
FIND_LIBRARY(libc++_LIBRARIES NAMES c++ HINTS ${libc++_ROOT}/lib)
FIND_LIBRARY(libc++abi_LIBRARIES NAMES c++abi HINTS ${libc++_ROOT}/lib)

# Handle the QUIETLY and REQUIRED arguments and set libc++_FOUND to TRUE if
# all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libc++ DEFAULT_MSG libc++_LIBRARIES libc++abi_LIBRARIES libc++_INCLUDES)

MARK_AS_ADVANCED(libc++_INCLUDES libc++_LIBRARIES libc++abi_LIBRARIES)
