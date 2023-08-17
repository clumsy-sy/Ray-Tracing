CC = g++
CFLAGS = -O3 -march=native -std=c++20 -Wall -Wextra

# SRC = $(wildcard src/*.cpp)
SRC += $(wildcard src/vector/*.cpp)
OBJ = $(patsubst %.cpp, %.o, ${SRC})

TARGET := build/RayTracing

.PHONY : test
test : ${OBJ}
${OBJ}:%.o:%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY : pbuild
pbuild : stb_image_o stb_image_gch stb_image_write_o stb_image_write_gch objLoader vec3dx4 otherHpp

stb_image_o: src/external/rtw_stb_image.cpp
	$(CC) -c src/external/rtw_stb_image.cpp -o build/stb_image.o $(CFLAGS)
stb_image_gch: src/external/rtw_stb_image.hpp
	$(CC) src/external/rtw_stb_image.hpp $(CFLAGS)

stb_image_write_o: src/external/rtw_stb_image_write.cpp
	$(CC) -c src/external/rtw_stb_image_write.cpp -o build/stb_image_write.o $(CFLAGS)
stb_image_write_gch: src/external/rtw_stb_image_write.h
	$(CC) src/external/rtw_stb_image_write.h $(CFLAGS)

objLoader: src/external/OBJ_Loader.hpp
		$(CC) src/external/OBJ_Loader.hpp $(CFLAGS)

vec3dx4 : src/vector/vec3dx4.h
	$(CC) src/vector/vec3dx4.h $(CFLAGS)

otherHpp : src/external/BMP.hpp
	$(CC) src/external/BMP.hpp $(CFLAGS)
	

.PHONY : build
build : 
	$(CC) -c src/main.cpp -o build/RayTracing.o $(CFLAGS)
	$(CC) build/RayTracing.o build/stb_image.o build/stb_image_write.o -o build/RayTracing

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

.PHONY : fmt
fmt:
	clang-format -i $(shell find src -name '*.cpp')
	clang-format -i $(shell find src -name '*.hpp')

.PHONY : clean
clean:
	rm src/*.o

.PHONY : buildclean
buildclean:
	cd build/ && rm -rf *

.PHONY : convert
convert:
	convert  xxx.jpg  xxx.png

