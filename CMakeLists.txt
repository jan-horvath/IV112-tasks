cmake_minimum_required(VERSION 3.8)
project(IV112-tasks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(Task_1)
add_subdirectory(Task_2)
add_subdirectory(Task_3)
add_subdirectory(Task_4)
add_subdirectory(Task_5)
add_subdirectory(Task_6)
add_subdirectory(Task_7)
add_subdirectory(Task_8)
add_subdirectory(Task_9)


set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv/build/)
find_package( OpenCV REQUIRED )

add_executable(task1 Task_1/task1.cpp)
add_executable(task2 Task_2/task2.cpp)
add_executable(task3 Task_3/task3.cpp)
add_executable(task4 Task_4/task4.cpp)
add_executable(task5 Task_5/task5.cpp)
add_executable(task6 Task_6/task6.cpp)
add_executable(task7 Task_7/task7.cpp)
add_executable(task8 Task_8/task8.cpp)
add_executable(task9 Task_9/task9.cpp)

target_link_libraries(task1 ${OpenCV_LIBS})
#target_link_libraries(task2 ${OpenCV_LIBS})
#target_link_libraries(task3 ${OpenCV_LIBS})
target_link_libraries(task4 ${OpenCV_LIBS})
#target_link_libraries(task5 ${OpenCV_LIBS})
target_link_libraries(task6 ${OpenCV_LIBS})
target_link_libraries(task7 ${OpenCV_LIBS})
#target_link_libraries(task8 ${OpenCV_LIBS})
#target_link_libraries(task9 ${OpenCV_LIBS})


