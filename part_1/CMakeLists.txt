cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(overlay_icon_p1 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define preprocessor definitions
add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX)

# Add the source files
set(SOURCES
    src/main.cpp
)

# Add an executable
add_executable(overlay_icon_p1 ${SOURCES})

# Link necessary libraries
target_link_libraries(overlay_icon_p1 PRIVATE user32 gdi32 shell32 gdiplus comctl32 Msimg32)

# Set output directory for all build types (Debug, Release, MinSizeRel, RelWithDebInfo)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set_target_properties(overlay_icon_p1 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_SOURCE_DIR}/bin/${OUTPUTCONFIG}"
        LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_SOURCE_DIR}/bin/${OUTPUTCONFIG}"
        ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_SOURCE_DIR}/bin/${OUTPUTCONFIG}"
        WIN32_EXECUTABLE YES
    )
endforeach()

# Ensure the directory exists
add_custom_command(TARGET overlay_icon_p1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:overlay_icon_p1>/files"
)

# Copy 'files' directory to the output directory after building
add_custom_command(TARGET overlay_icon_p1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/files" "$<TARGET_FILE_DIR:overlay_icon_p1>/files"
)
