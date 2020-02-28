# src/CMakeLists.txt
# ------------------

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# target executable
add_executable(comtort WIN32 "comtort.cpp" "comtort.h" 
               "menu.cpp" "menu.h" "canvas.cpp" "canvas.h" 
			   "node.cpp" "node.h" "project.cpp" "project.h")

# target linking
target_link_libraries(comtort ${wxWidgets_LIBRARIES})