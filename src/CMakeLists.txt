# src/CMakeLists.txt
# ------------------

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# glm
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# target executable
add_executable(comtort WIN32 "comtort.cpp" "comtort.h" 
               "menu.cpp" "menu.h" "canvas.cpp" "canvas.h" 
			   "node.cpp" "node.h" "project.cpp" "project.h"
			   "camera.h" "camera.cpp" "toolstate.h" "toolstate.cpp"
			   "tool.h" "tool.cpp")

# target linking
target_link_libraries(comtort ${wxWidgets_LIBRARIES})