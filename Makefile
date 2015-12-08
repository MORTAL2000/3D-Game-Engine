CXX := g++
SRC_DIR := "./"
API := WINDOWS

MODULE := Engine
BUILD_TYPE := ENGINE

BUILD_DIR := build
SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SOURCES))
FINAL_OBJS :=  $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))

INCLUDES := -I. -I"./libs" -I"./libs/lodepng/" -I"./libs/lua/" -I"./libs/freetype/include"
INCLUDES += -I"./libs/glew/include/GL" -I"./libs/bullet3/src" -I"./libs/glfw3/include"
INCLUDES += -I"./libs/imgui" -I"./libs/openal-soft/include" -I"./libs/tinyobjloader"

LUA := -L"./libs/lua/" -llua
GLFW := -L"./libs/glfw3/src" -lglfw3
GLEW := -L"./libs/glew/lib" -lglew32
OPENGL := -lopengl32
BULLET := -L"./libs/bullet3" -lBulletDynamics -lBulletCollision -lLinearMath
AUDIO := -L"./libs/openal-soft" -lcommon -lOpenAL32.dll -lwinmm
FREETYPE := -L"./libs/freetype" -lfreetype
STDLIBS := -lcomdlg32 -lole32 -lgdi32 -lgomp -lwsock32

LIBS := $(GLFW) $(GLEW) $(OPENGL) $(LUA) $(BULLET) $(AUDIO) $(FREETYPE) -lstdc++ $(STDLIBS)

CXXFLAGS := -Wall -Wextra -Wunreachable-code -Winline -Wcast-align -static -static-libgcc -static-libstdc++ -O2
LDFLAGS := -std=c++11 -DGLEW_STATIC -DBUILD_$(BUILD_TYPE)
DEL_FILE = rm -f

all: $(MODULE)

$(MODULE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(FINAL_OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(LDFLAGS) $(INCLUDES) -c $< -o $(addprefix $(BUILD_DIR)/, $(notdir $@))

clean:
	rm -f $(FINAL_OBJS)

run:
	./$(MODULE)
