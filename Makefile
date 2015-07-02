CXX := g++
SRC_DIR := "./"
API := WINDOWS

MODULE := Engine
BUILD_TYPE := ENGINE

BUILD_DIR := build
SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SOURCES))
FINAL_OBJS :=  $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))

INCLUDES := -I. -I"./dependencies" -I"./dependencies/lodepng/" -I"./dependencies/lua/" -I"./dependencies/freetype/include"
INCLUDES += -I"./dependencies/glew/include/GL" -I"./dependencies/bullet3/src" -I"./dependencies/glfw3/include"
INCLUDES += -I"./dependencies/imgui" -I"./dependencies/openal-soft/include"

LUA := -L"./dependencies/lua/" -llua
GLFW := -L"./dependencies/glfw3/src" -lglfw3
GLEW := -L"./dependencies/glew/lib" -lglew32
OPENGL := -lopengl32
BULLET := -L"./dependencies/bullet3/bin" -lBulletDynamics_gmake -lBulletCollision_gmake -lLinearMath_gmake
AUDIO := -L"./dependencies/openal-soft/build" -lcommon -lOpenAL32.dll -lwinmm
FREETYPE := -L"./dependencies/freetype" -lfreetype
STDLIBS := -lcomdlg32 -lole32 -lgdi32 -lgomp -lwsock32

LIBS := $(GLFW) $(GLEW) $(OPENGL) $(LUA) $(BULLET) $(AUDIO) $(FREETYPE) -lstdc++ $(STDLIBS)

CXXFLAGS := -O3 -Wall -Wextra -Wunreachable-code -Winline -Wcast-align -static -static-libgcc -static-libstdc++
LDFLAGS := -std=c++0x -DGLEW_STATIC -DBUILD_$(BUILD_TYPE)
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
