
set(NAME application)

if (WIN32 AND BUILD_GUI)
    add_executable(${NAME} WIN32)
else()
    add_executable(${NAME})
endif()

file(GLOB_RECURSE
    PROJECT_SOURCE_FILES CONFIGURE_DEPENDS
    "src/*.cpp" "src/*.c"
)
file(GLOB_RECURSE
    PROJECT_HEADER_FILES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.h"
)

target_compile_features(${NAME} PUBLIC c_std_11)
target_compile_definitions(${NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${NAME} PUBLIC -Wall)
endif()

target_sources(${NAME} PRIVATE
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_HEADER_FILES}
)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake_targets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "sources"
    FILES ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES}
)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(${NAME} PUBLIC ${MATH_LIBRARY})
endif()

#-------------------------------------------------------------------------------
# Third party libraries

target_link_libraries(${NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

#-------------------------------------------------------------------------------
# Other include directories

target_include_directories(
    ${NAME} PUBLIC
    "src"
)

#-------------------------------------------------------------------------------
# Copy DLL and files

if(MSVC)
    function(copy_dll PATH_TO_DLL)
        foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
            message(STATUS "[INFO] Copy: " ${PATH_TO_DLL} " to " ${CONFIG})
            file(
                COPY ${PATH_TO_DLL}
                DESTINATION "${CMAKE_SOURCE_DIR}/_bin/${CONFIG}"
            )
        endforeach(CONFIG)
    endfunction()
    copy_dll("${THIRD_PARTY_SDL3_DIR}/lib/x64/SDL3.dll")
    copy_dll("${THIRD_PARTY_SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll")
    copy_dll("${THIRD_PARTY_SDL3_TTF_DIR}/lib/x64/SDL3_ttf.dll")
endif()
