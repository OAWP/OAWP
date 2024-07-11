# Partially written by ChatGPT

function(add_git_hash)
    # Get the short Git commit hash
    execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH_SHORT_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Get the long Git commit hash
    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH_LONG_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # Check if the short hash result is empty and set a default value if necessary
    if(NOT GIT_HASH_SHORT_RESULT)
        set(GIT_HASH_SHORT_RESULT "N/A")
    endif()

    # Check if the long hash result is empty and set a default value if necessary
    if(NOT GIT_HASH_LONG_RESULT)
        set(GIT_HASH_LONG_RESULT "N/A")
    endif()

    # Define the compile definitions
    add_compile_definitions(
        GIT_HASH_SHORT="${GIT_HASH_SHORT_RESULT}"
        GIT_HASH_LONG="${GIT_HASH_LONG_RESULT}"
    )

endfunction()
