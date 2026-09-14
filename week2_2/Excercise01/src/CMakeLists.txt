set (INC ${PROJECT_SOURCE_DIR}/inc)

set(SRC main.cpp first_text.cpp second_text.cpp nerd.cpp)

add_executable(main.out ${SRC})

target_include_directories(main.out PUBLIC ${INC})

target_sources(main.out PUBLIC ${SRC})