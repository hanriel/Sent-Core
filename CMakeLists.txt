cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(Sent-Core
        LANGUAGES C CXX
        VERSION 0.0.102
        DESCRIPTION "Official Sent server"
        HOMEPAGE_URL "https://sent.hanriel.ru"
)

add_subdirectory(lib)
add_subdirectory(src)