CXX = g++

ifeq ($(OS),Windows_NT)
    GLFW_LIB = -lglfw3 -lopengl32 -lglu32 -lfreeglut
    GLFW_LIB_DIR = dll/copasFolder/dependencies/lib
    GLFW_INCLUDE_DIR = dll/copasFolder/dependencies/include
endif

CXXFLAGS = -Wall -std=c++17 -g \
    -I$(GLFW_INCLUDE_DIR) \
    -I/ucrt64/include \
    -Iloadimage

LDFLAGS = -L/ucrt64/lib -L$(GLFW_LIB_DIR) $(GLFW_LIB)

HEADERS = \
    Block.h Mat3.h Player.h renderChunks.h Renderer.h \
    textureManager.h Vector3.h loadimage/textureloader.h

SRCS = \
    main.cpp Renderer.cpp renderChunks.cpp textureManager.cpp \
    loadimage/textureloader.cpp dll/copasFolder/glad.c

OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

TARGET = blockgame

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
