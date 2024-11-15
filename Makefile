.PHONY: all build conan cmake compile clean run test

all: build

build: conan cmake compile

conan:
	conan install . --output-folder=build --build=missing

cmake:
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

compile:
	cd build && cmake --build .

clean:
	rm -rf build/*

run: build
	./build/swcheck -f .deployer/config/conf.yaml -l log.txt

test: build
	cd build && ctest --output-on-failure
	# или для более подробного вывода:
	# cd build && ./unit_tests --gtest_color=yes

help:
	@echo "Доступные команды:"
	@echo "  make         - полная сборка проекта (то же, что и make build)"
	@echo "  make build   - полная сборка проекта (conan + cmake + compile)"
	@echo "  make conan   - установка зависимостей через conan"
	@echo "  make cmake   - генерация CMake файлов"
	@echo "  make compile - компиляция проекта"
	@echo "  make clean   - очистка сборочных файлов"
	@echo "  make run     - сборка и запуск проекта"
	@echo "  make test    - запуск тестов"