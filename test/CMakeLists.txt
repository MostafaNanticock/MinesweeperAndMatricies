enable_testing()

include_directories( "${PROJECT_SOURCE_DIR}/src" )

add_executable( run-all-tests 
   run-tests.c
   testmatrix.c
   testvector.c
   )
target_link_libraries( run-all-tests
   matrix
   vector
   )

add_test( AllTests run-all-tests )
