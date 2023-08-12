CC = g++
CFLAGS = -O3 -march=native -std=c++20 -Wall -Wextra

SRC = $(wildcard src/*.cu)
SRC += $(wildcard src/vector/*.cu)
OBJ = $(patsubst %.cu, %.o, ${SRC})

TARGET := build/RayTracing

.PHONY : pbuild
pbuild : 
	$(CC) -c src/external/rtw_stb_image.cpp -o build/stb_image.o $(CFLAGS)
	$(CC) src/vector/Vec3dx4.hpp $(CFLAGS)
	$(CC) src/global.hpp $(CFLAGS)
	$(CC) src/external/OBJ_Loader.hpp $(CFLAGS)

.PHONY : build
build : 
	$(CC) -c src/main.cpp -o build/RayTracing.o $(CFLAGS)
	$(CC) build/RayTracing.o build/stb_image.o -o build/RayTracing

.PHONY : run
run : $(TARGET)
	./$(TARGET)

.PHONY : cmrun
cmrun:
	cmake --build build --config Release --parallel $(nproc)
	./$(TARGET)

.PHONY : perf
perf: $(TARGET)
	nvprof ./$(TARGET)

.PHONY : build
build: $(TARGET)

.PHONY : fmt
fmt:
	clang-format -i $(shell find src -name '*.cpp')
	clang-format -i $(shell find src -name '*.hpp')

.PHONY : clean
clean:
	rm src/*.o

.PHONY : buildclean
buildclean:
	cd src/build && rm -rf *

.PHONY : convert
convert:
	convert  xxx.jpg  xxx.png

