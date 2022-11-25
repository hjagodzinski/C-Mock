enable_testing()
include(GoogleTest)

# -------------------------------------------------------------------
add_library( math_api INTERFACE )
add_library( math::api ALIAS math_api )

target_include_directories(math_api
    INTERFACE
        .
)

add_library( math_static STATIC)
target_sources(math_static
    PRIVATE
        math.h
        math.c
)
target_link_libraries(math_static
    PUBLIC
        math::api
)

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
# Static library using interface test.
# -------------------------------------------------------------------
add_executable( class_mockers_test_static )

target_sources( class_mockers_test_static
    PRIVATE
        cmock-function-class-mockers_test.cc
)

target_link_libraries( class_mockers_test_static
    PRIVATE
        math::api
        gmock_c
        gmock
        gmock_main
)

gtest_add_tests( TARGET class_mockers_test_static )

# -------------------------------------------------------------------
add_executable( class_mockers_old_style_test_static )

target_sources( class_mockers_old_style_test_static
    PRIVATE
        cmock-function-class-mockers-old-style_test.cc
)

target_link_libraries( class_mockers_old_style_test_static
    PRIVATE
        math::api
        gmock_c
        gmock
        gmock_main
)

gtest_add_tests( TARGET class_mockers_old_style_test_static )

# -------------------------------------------------------------------
add_executable( function_mockers_test_static )

target_sources( function_mockers_test_static
    PRIVATE
        cmock-function-mockers_test.cc
)

target_link_libraries( function_mockers_test_static
    PRIVATE
        math::api
        gmock_c
        gmock
        gmock_main
)

gtest_add_tests( TARGET function_mockers_test_static )

# -------------------------------------------------------------------
add_executable( spec_builders_test_static )

target_sources( spec_builders_test_static
    PRIVATE
        cmock-spec-builders_test.cc
)

target_link_libraries( spec_builders_test_static
    PRIVATE
        math::api
        gmock_c
        gmock
        gmock_main
)

gtest_add_tests( TARGET spec_builders_test_static )


# -------------------------------------------------------------------
# Shared (Dynamic) library using interface test.
# -------------------------------------------------------------------
# TODO: Figure out where math_shared and link it appropriately

# add_executable( class_mockers_test_shared )

# target_sources( class_mockers_test_shared
#     PRIVATE
#         cmock-function-class-mockers_test.cc
# )

# target_compile_definitions(class_mockers_test_shared
#     PRIVATE
#         SUPPORT_REAL_FUNCTIONS=1
# )

# target_link_libraries( class_mockers_test_shared
#     PRIVATE
#         math_shared
#         gmock_c
#         gmock
#         gmock_main
# )

# gtest_add_tests( TARGET class_mockers_test_shared )

# # -------------------------------------------------------------------
# add_executable( class_mockers_old_style_test_shared )

# target_sources( class_mockers_old_style_test_shared
#     PRIVATE
#         cmock-function-class-mockers-old-style_test.cc
# )

# target_compile_definitions(class_mockers_old_style_test_shared
#     PRIVATE
#         SUPPORT_REAL_FUNCTIONS=1
# )

# target_link_libraries( class_mockers_old_style_test_shared
#     PRIVATE
#         math_shared
#         gmock_c
#         gmock
#         gmock_main
# )

# gtest_add_tests( TARGET class_mockers_old_style_test_shared )

# # -------------------------------------------------------------------
# add_executable( function_mockers_test_shared )

# target_sources( function_mockers_test_shared
#     PRIVATE
#         cmock-function-mockers_test.cc
# )

# target_compile_definitions(function_mockers_test_shared
#     PRIVATE
#         SUPPORT_REAL_FUNCTIONS=1
# )

# target_link_libraries( function_mockers_test_shared
#     PRIVATE
#         math_shared
#         gmock_c
#         gmock
#         gmock_main
# )

# gtest_add_tests( TARGET function_mockers_test_shared )

# # -------------------------------------------------------------------
# add_executable( spec_builders_test_shared )

# target_sources( spec_builders_test_shared
#     PRIVATE
#         cmock-spec-builders_test.cc
# )

# target_compile_definitions(spec_builders_test_shared
#     PRIVATE
#         SUPPORT_REAL_FUNCTIONS=1
# )

# target_link_libraries( spec_builders_test_shared
#     PRIVATE
#         math_shared
#         gmock_c
#         gmock
#         gmock_main
# )

# gtest_add_tests( TARGET spec_builders_test_shared )