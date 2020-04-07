# src/CMakeLists.txt
# ------------------

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# target executable
add_executable(comtort WIN32 "comtort.h" "comtort.cpp"
	"node.h" "node.cpp" "wire.h" "wire.cpp"
	"network.h" "network.cpp" "project.h" "project.cpp"
	"tool.h" "tool.cpp" "camera.h" "camera.cpp" 
	"input_defs.h" "draw_defs.h" "math_defs.h"
	"math_defs.cpp" "canvas.h" "canvas.cpp" 
	"book.h" "book.cpp" "page.h" "page.cpp")

# target linking
target_link_libraries(comtort ${wxWidgets_LIBRARIES})