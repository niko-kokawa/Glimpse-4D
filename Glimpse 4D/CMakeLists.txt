cmake_minimum_required(VERSION 3.12)
project(Glimpse4D LANGUAGES CXX)

# =========================
# Executable
# =========================
add_executable(Glimpse4D
    "Glimpse 4D.cpp"
    "Glimpse 4D.h"
    "MeshData.cpp"
    "MeshData.h"
    "glad/src/glad.c"
 "Camera.h" "Camera.cpp" "Space.h" "Space.cpp")

target_compile_definitions(Glimpse4D PRIVATE Py_NO_ENABLE_SHARED)


target_compile_features(Glimpse4D PRIVATE cxx_std_20)

# =========================
# Include directories
# =========================
target_include_directories(Glimpse4D PRIVATE
    "${CMAKE_SOURCE_DIR}/Glimpse 4D/glfw/include"
    "${CMAKE_SOURCE_DIR}/Glimpse 4D/glad/include"
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# =========================
# GLFW / OpenGL / GLM
# =========================
target_link_libraries(Glimpse4D PRIVATE
    "${CMAKE_SOURCE_DIR}/Glimpse 4D/glfw/lib-vc2022/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/glm.lib"
    opengl32
)

# Copy GLFW DLL
add_custom_command(TARGET Glimpse4D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Glimpse 4D/glfw/lib-vc2022/glfw3.dll"
        $<TARGET_FILE_DIR:Glimpse4D>
)

# =========================
# Python (pyenv, Release-only, NO _d)
# =========================
set(PYTHON_ROOT "$ENV{USERPROFILE}/.pyenv/pyenv-win/versions/3.13.2")

# IMPORTANT:
# Treat Python as a STATIC imported library
# This completely avoids python313_d.lib forever
add_library(python313 STATIC IMPORTED)

set_target_properties(python313 PROPERTIES
    IMPORTED_LOCATION "${PYTHON_ROOT}/libs/python313.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PYTHON_ROOT}/include"
)

target_link_libraries(Glimpse4D PRIVATE python313)

# =========================
# Python venv + pip (FIXED)
# =========================
# Never use "activate" in CMake.
# Always call the venv python directly.

if(WIN32)
    add_custom_command(TARGET Glimpse4D POST_BUILD
        COMMAND "${PYTHON_ROOT}/python.exe" -m venv "$<TARGET_FILE_DIR:Glimpse4D>/venv"
        COMMAND "$<TARGET_FILE_DIR:Glimpse4D>/venv/Scripts/python.exe"
                -m pip install -r "${CMAKE_SOURCE_DIR}/requirements.txt"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
else()
    add_custom_command(TARGET Glimpse4D POST_BUILD
        COMMAND "${PYTHON_ROOT}/bin/python" -m venv "$<TARGET_FILE_DIR:Glimpse4D>/venv"
        COMMAND "$<TARGET_FILE_DIR:Glimpse4D>/venv/bin/python"
                -m pip install -r "${CMAKE_SOURCE_DIR}/requirements.txt"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()


#
# Adding python dlls into the project
# 
#
#
add_custom_command(TARGET Glimpse4D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PYTHON_ROOT}/python313.dll
    $<TARGET_FILE_DIR:Glimpse4D>
)