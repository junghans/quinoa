

TRIBITS_ADD_EXECUTABLE_AND_TEST(
  MultiVector_UnitTests
  SOURCES
    MultiVector_UnitTests
    ${TEUCHOS_STD_UNIT_TEST_MAIN}
  COMM serial mpi
  STANDARD_PASS_OUTPUT
  )

# ##### temporarily disabling this test
# TRIBITS_ADD_EXECUTABLE_AND_TEST(
#   MultiVector_UnitTests
#   SOURCES
#     MultiVector_UnitTests
#     ${TEUCHOS_STD_UNIT_TEST_MAIN}
#   COMM serial mpi
#   STANDARD_PASS_OUTPUT
#   )

IF (NOT Xpetra_Epetra_NO_32BIT_GLOBAL_INDICES)
  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    MultiVector_UnitTestsXpetraSpecific
    SOURCES
      MultiVector_UnitTestsXpetraSpecific
      ${TEUCHOS_STD_UNIT_TEST_MAIN}
    COMM serial mpi
    STANDARD_PASS_OUTPUT
    )
ENDIF()


IF (NOT Xpetra_Epetra_NO_64BIT_GLOBAL_INDICES)
  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    MultiVector_UnitTestsXpetraSpecific_LL
    SOURCES
      MultiVector_UnitTestsXpetraSpecific
      ${TEUCHOS_STD_UNIT_TEST_MAIN}
    COMM serial mpi
    STANDARD_PASS_OUTPUT
    DEFINES -DXPETRA_TEST_USE_LONGLONG_GO
    )
ENDIF()

IF (HAVE_XPETRA_THYRA)
	IF (NOT Xpetra_Epetra_NO_32BIT_GLOBAL_INDICES)
	  TRIBITS_ADD_EXECUTABLE_AND_TEST(
	    MultiVector_UnitTestsThyraSpecific
	    SOURCES
	      MultiVector_UnitTestsThyraSpecific
	      ${TEUCHOS_STD_UNIT_TEST_MAIN}
	    COMM serial mpi
	    STANDARD_PASS_OUTPUT
	    )
	ENDIF()
ENDIF()

