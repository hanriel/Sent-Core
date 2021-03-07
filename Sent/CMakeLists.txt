configure_file(
        config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set(SOURCES
        main.cpp
        TcpServer.cpp
        Message.cpp
        tiny_rsa.cpp
        Logger.cpp
)

set(HEADERS
        TcpServer.h
        Message.h
        User.h
        tiny_rsa.h
        Logger.h
)

add_executable(Sent ${SOURCES} ${HEADERS})
target_include_directories(Sent PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(ThirdParty/spdlog)

target_link_libraries(Sent spdlog)