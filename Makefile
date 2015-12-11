# Makefile building the engine and the frontend editor
# @author Alexander Koch

CXX := g++
SRC_DIR := "./"
API := WINDOWS

# MODULE -> Executable name
# BUILD_TYPE -> ENGINE / EDITOR
MODULE := Engine
BUILD_TYPE := ENGINE

# Source and *.o conversion
BUILD_DIR := build
SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SOURCES))
FINAL_OBJS :=  $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))

INCLUDES := -I. -I"./libs" -I"./libs/lodepng/" -I"./libs/lua/" -I"./libs/freetype/include"
INCLUDES += -I"./libs/glew/include/GL" -I"./libs/bullet3/src" -I"./libs/glfw3/include"
INCLUDES += -I"./libs/imgui" -I"./libs/tinyobjloader"

# All the libraries
LUA := -L"./libs/lua/" -llua
GLFW := -L"./libs/glfw3/src" -lglfw3
GLEW := -L"./libs/glew/lib" -lglew32
BULLET := -L"./libs/bullet3" -lBulletDynamics -lBulletCollision -lLinearMath
FREETYPE := -L"./libs/freetype" -lfreetype
STDLIBS := -lcomdlg32 -lole32 -lgdi32 -lgomp -lwsock32
LIBS := $(GLFW) $(GLEW) -lopengl32 $(LUA) $(BULLET) $(FREETYPE) -lstdc++ $(STDLIBS)

# Flags
CXXFLAGS := -Wall -Wextra -Wunreachable-code -Winline -Wcast-align -static -static-libgcc -static-libstdc++ -O2
LDFLAGS := -std=c++11 -DGLEW_STATIC -DBUILD_$(BUILD_TYPE)
DEL_FILE = rm -f

# Definitions
all: $(MODULE)

# Compile object files
$(MODULE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(FINAL_OBJS) $(LIBS)

# Compile source to object files
%.o: %.cpp
	$(CXX) $(LDFLAGS) $(INCLUDES) -c $< -o $(addprefix $(BUILD_DIR)/, $(notdir $@))

# Remove the object files
clean:
	rm -f $(FINAL_OBJS)

# Execute the module
run:
	./$(MODULE)
