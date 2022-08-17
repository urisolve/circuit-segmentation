# Fetch dependencies

cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# Fetch googletest
function(fetch_googletest)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endfunction()

# Fetch stduuid
function(fetch_stduuid)
    FetchContent_Declare(
        stduuid
        GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
        GIT_TAG 3afe7193facd5d674de709fccc44d5055e144d7a
    )
    FetchContent_MakeAvailable(stduuid)
endfunction()

# Fetch json
function(fetch_json)
    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(json)
endfunction()

# GoogleTest
if (BUILD_TESTS)
    fetch_googletest()
endif()

# stduuid
fetch_stduuid()

# json
fetch_json()
