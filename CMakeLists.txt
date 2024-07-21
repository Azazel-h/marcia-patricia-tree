cmake_minimum_required(VERSION 3.28)
project(marcia-patricia-tree)

# here you can see how instead of writing "SomeLibrary"
# we can just use the PROJECT_NAME variable
add_library(${PROJECT_NAME} STATIC)

# no need to add headers here, only sources are required
target_sources(${PROJECT_NAME}
        PRIVATE
        src/some.cpp
)