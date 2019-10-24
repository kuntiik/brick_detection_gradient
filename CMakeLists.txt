
set(EXEC_NAME grad_detection.cpp)
set(OUTPUT_NAME grad_detection)
cmake_minimum_required(VERSION 2.8)
project( ${OUTPUT_NAME})
find_package( OpenCV REQUIRED )
add_executable( ${OUTPUT_NAME}  ${EXEC_NAME})
target_link_libraries(${OUTPUT_NAME} ${OpenCV_LIBS} )
