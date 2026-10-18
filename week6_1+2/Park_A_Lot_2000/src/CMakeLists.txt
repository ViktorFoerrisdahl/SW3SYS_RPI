set (INC ${PROJECT_SOURCE_DIR}/inc)

set(SRC Park_A_Lot_2000.cpp car.cpp gate.cpp)

add_executable(Park_A_Lot_2000 ${SRC})

target_include_directories(Park_A_Lot_2000 PUBLIC ${INC})

target_sources(Park_A_Lot_2000 PUBLIC ${SRC})
