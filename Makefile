# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g
LDFLAGS = -lopengl32 -lglu32 -lfreeglut

# Source and object files
SRCS = main.cpp Renderer.cpp renderChunks.cpp Player.cpp \
       textureManager.cpp loadimage/textureloader.cpp
OBJS = $(SRCS:.cpp=.o)

# Headers (optional: for dependencies)
HEADERS = Block.h Mat3.h Player.h renderChunks.h Renderer.h Vector3.h \
          textureManager.h loadimage/textureloader.h loadimage/stb_image.h

TARGET = Blockcraft

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
