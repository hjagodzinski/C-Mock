enable_testing()
include(GoogleTest)

# -------------------------------------------------------------------
add_library( math_shared SHARED )

target_sources(math_shared
    PRIVATE
        math.h
        math.c
)
target_include_directories(math_shared
    PUBLIC
        .
)

# install libs
install(TARGETS math_shared
    LIBRARY DESTINATION lib
)

# -------------------------------------------------------------------
add_executable( class_mockers_test )

target_sources( class_mockers_test
    PRIVATE
        cmock-function-class-mockers_test.cc
)

target_link_libraries( class_mockers_test
    PRIVATE
        math_shared
        gmock_c
        gmock
        gmock_main
)

gtest_discover_tests( class_mockers_test )


# -------------------------------------------------------------------
add_executable( class_mockers_old_style_test )

target_sources( class_mockers_old_style_test
    PRIVATE
        cmock-function-class-mockers-old-style_test.cc
)

target_link_libraries( class_mockers_old_style_test
    PRIVATE
        math_shared
        gmock_c
        gmock
        gmock_main
)

gtest_discover_tests( class_mockers_old_style_test )

# -------------------------------------------------------------------
add_executable( function_mockers_test )

target_sources( function_mockers_test
    PRIVATE
        cmock-function-mockers_test.cc
)

target_link_libraries( function_mockers_test
    PRIVATE
        math_shared
        gmock_c
        gmock
        gmock_main
)

gtest_discover_tests( function_mockers_test )

# -------------------------------------------------------------------
add_executable( spec_builders_test )

target_sources( spec_builders_test
    PRIVATE
        cmock-spec-builders_test.cc
)

target_link_libraries( spec_builders_test
    PRIVATE
        math_shared
        gmock_c
        gmock
        gmock_main
)

gtest_discover_tests( spec_builders_test )
