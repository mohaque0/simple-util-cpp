SRC_DIR=src
VERSION=0.1

release: BUILD_DIR=build/release
debug: BUILD_DIR=build/debug
debug: CMAKE_FLAGS= -DCMAKE_BUILD_TYPE=Debug

conan:
	conan create . Util/${VERSION}@util/stable

release: build_system
	${MAKE} -C ${BUILD_DIR} -s

debug: build_system
	${MAKE} -C ${BUILD_DIR} -s

.PHONY: test
test:
	conan test test Util/${VERSION}@util/stable

filelist: build_dir
	rm -f ${BUILD_DIR}/filelist.txt
	find src -name *.cpp >> ${BUILD_DIR}/filelist.txt
	find src -name *.hpp >> ${BUILD_DIR}/filelist.txt

build_system: CMakeLists.txt filelist build_dir
	cd ${BUILD_DIR}; cmake ${CMAKE_FLAGS} ../../

build_dir:
	mkdir -p ${BUILD_DIR}

doc:
	doxygen Doxyfile

clobber:
	rm -rf build
