CXX = g++
CXXFLAGS = -Wall -std=c++17 -g \
    -I/ucrt64/include \
    -Idll/copasFolder/dependencies/include \
    -Iloadimage

LDFLAGS = -L/ucrt64/lib -L/dll/copasFolder/dependencies/lib \
          -lglfw3 -lopengl32 -lglu32 -lfreeglut  # Hapus -lglad karena pakai glad.o langsung

HEADERS = \
    Block.h \
    Mat3.h \
    Player.h \
    renderChunks.h \
    Renderer.h \
    loadimage/textureloader.h

SRCS = \
    main.cpp \
    Renderer.cpp \
    renderChunks.cpp \
    loadimage/textureloader.cpp \
    dll/copasFolder/glad.c  # glad.c tetap di sini

OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)  # Konversi glad.c → glad.o

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
