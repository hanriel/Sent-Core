cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)

project(Sent
        LANGUAGES C CXX
        VERSION 0.0.102
        DESCRIPTION "Official Sent server"
        HOMEPAGE_URL "https://sent.hanriel.ru"
        )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sent)
get_filename_component(third_party_loc "Sent/ThirdParty" REALPATH)
get_filename_component(submodules_loc "Sent" REALPATH)

add_subdirectory(Sent)