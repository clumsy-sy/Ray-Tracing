fmt:
	clang-format -i `find src -name '*.cpp'`
	clang-format -i `find src -name '*.hpp'`

clean:
	cd src/build && rm -rf *

bpy:
	python3 build.py

pre-build:
	g++ -c src/external/rtw_stb_image.cpp -o build/stb_image.o -O3 -march=native -std=c++20  -Wall -Wextra
	g++ src/vector/Vec3dx4.hpp -O3 -march=native -std=c++20  -Wall -Wextra
	g++ src/global.hpp -O3 -march=native -std=c++20  -Wall -Wextra
	g++ src/external/OBJ_Loader.hpp -O3 -march=native -std=c++20  -Wall -Wextra

build:
	g++ -c src/main.cpp -o build/RayTracing.o -O3 -march=native -std=c++20  -Wall -Wextra
	g++ build/RayTracing.o build/stb_image.o -o build/RayTracing

debug:
	g++ src/main.cpp -o build/RayTracing -O3 -march=native -std=c++20

move:
	mv *.bmp images/

run:
	./build/RayTracing

convert:
	convert  xxx.jpg  xxx.png