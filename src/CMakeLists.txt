add_library( game game.cpp )

add_executable( mnm main.cpp )
target_link_libraries( mnm
   game
   )
