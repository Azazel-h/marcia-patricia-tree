# In tests/CMakeLists.txt
add_executable(merkle_patricia_tree_tests)


file(GLOB_RECURSE TESTS_SRC
        CONFIGURE_DEPENDS
        "common/*.cpp"
        "rlp/*.cpp"
        "trie/*.cpp"
)

target_sources(merkle_patricia_tree_tests
        PRIVATE
        ${TESTS_SRC}
)

target_link_libraries(merkle_patricia_tree_tests
        PRIVATE
        merkle-patricia-tree  # Your main library
        evmone
        Catch2::Catch2WithMain  # Catch2 library

)

# Include directories if needed
target_include_directories(merkle_patricia_tree_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/intx/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/ethash/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/evmc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/expected/include
)

# Optional: Discover tests
include(CTest)
include(Catch)
catch_discover_tests(merkle_patricia_tree_tests)