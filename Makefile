ERL_INCLUDE_PATH := $(shell erl -noshell -eval 'io:format("~s", [code:root_dir() ++ "/usr/include"])' -s init stop)

ifeq ($(strip $(ERL_INCLUDE_PATH)),)
	$(error Failed to determine ERL include path. Ensure Erlang is installed and accessible.)
endif

ifeq ($(BUILD_DIR),)
	$(error BUILD_DIR not specified, you probably called `make` directly, you should use `mix compile`.)
endif

TIDESDB_SRC_DIR := tidesdb
TIDESDB_BUILD_DIR := ${TIDESDB_SRC_DIR}/build

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIB_SUFFIX := "dylib"
endif
ifeq ($(UNAME_S),Linux)
	LIB_SUFFIX := "so"
endif

TIDESDB_LIB := ${TIDESDB_BUILD_DIR}/libtidesdb.${LIB_SUFFIX}
NIF_SRC := c_src/*.c
NIF_LIB := ${BUILD_DIR}/priv/tidesdb.so
CFLAGS := -Wall -fPIC -shared -dynamiclib -undefined dynamic_lookup -I $(ERL_INCLUDE_PATH) -I ${TIDESDB_BUILD_DIR}/include -L ${TIDESDB_BUILD_DIR} -l tidesdb -rpath ${TIDESDB_BUILD_DIR}

${NIF_LIB}: ${TIDESDB_LIB} ${NIF_SRC}
	@mkdir -p `dirname $(NIF_LIB)`
	clang $(CFLAGS) -o $(NIF_LIB) $(NIF_SRC)

${TIDESDB_LIB}:
	git submodule init ${TIDESDB_SRC_DIR}
	git submodule update ${TIDESDB_SRC_DIR}
	cmake -S ${TIDESDB_SRC_DIR} -B ${TIDESDB_BUILD_DIR} -DCMAKE_BUILD_TYPE=Release -DTIDESDB_BUILD_TESTS=OFF -DTIDESDB_WITH_TCMALLOC=ON
	cmake --build ${TIDESDB_BUILD_DIR}

#completions:
#	BUILD_DIR=_build/dev/lib/tidesdb/ bear -- make
# clean:
# 	rm -f ${TIDESDB_DYLIB}
# 	rm -rf ${TIDESDB_SRC_DIR}
