# Get required libraries
find_library(M_LIB m)

# Standalone Libraries (Axioms so to speak)
add_library( game game.cpp )

# Vector requires the math libraries
add_library( vector vector.c )
target_link_libraries(vector 
   ${M_LIB}
   )

# Matricies require vectors in my implementation
add_library( matrix matrix.c )
target_link_libraries( matrix
   vector
   ${M_LIB}
   )

# The actual game-solver itself
add_executable( mnm main.cpp )
target_link_libraries( mnm
   game
   )
