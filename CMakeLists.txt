cmake_minimum_required(VERSION 3.8)
project(IV112-tasks)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Task_1)
add_subdirectory(Task_2)

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv/build/)
find_package( OpenCV REQUIRED )

add_executable(task1 Task_1/task1.cpp)
add_executable(task2 Task_2/task2.cpp)
add_executable(task3 Task_3/task3.cpp)
add_executable(task4 Task_4/task4.cpp)

target_link_libraries(task1 ${OpenCV_LIBS})
#target_link_libraries(task2 ${OpenCV_LIBS})
#target_link_libraries(task3 ${OpenCV_LIBS})
target_link_libraries(task4 ${OpenCV_LIBS})


