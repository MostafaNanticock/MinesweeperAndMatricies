# Get required libraries
find_library(M_LIB m)

# Standalone Libraries (Axioms so to speak)
add_library( game SHARED game.cpp )

# Vector requires the math libraries
add_library( vector SHARED vector.cpp )
target_link_libraries(vector 
   ${M_LIB}
   )

# Matricies require vectors in my implementation
add_library( matrix SHARED matrix.cpp )
target_link_libraries( matrix
   vector
   ${M_LIB}
   )

add_library( game-solver SHARED solver.cpp )
target_link_libraries( game-solver
   matrix
   game
   )

# The actual game-solver itself
add_executable( mnm main.cpp )
target_link_libraries( mnm
   game
   game-solver
   )
