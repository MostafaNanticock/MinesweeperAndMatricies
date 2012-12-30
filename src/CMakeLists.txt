# Standalone Libraries (Axioms so to speak)
add_library( game game.cpp )
add_library( vector vector.c )

# Matricies require vectors in my implementation
add_library( matrix matrix.c )
target_link_libraries( matrix
   vector
   )

# The actual game-solver itself
add_executable( mnm main.cpp )
target_link_libraries( mnm
   game
   )
