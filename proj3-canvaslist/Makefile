CXX = clang++
# Unused: warn, but annoying to block compilation on
# Sign compare: noisy
# Command line arg: noisy, not relevant to students
CXXFLAGS = \
	-Wall -Wextra -Werror \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=unused-variable \
	-Wno-error=unused-but-set-variable \
	-Wno-error=unused-value \
	-Wno-sign-compare \
	-Wno-unused-command-line-argument \
	-std=c++2a -I. -g -fno-omit-frame-pointer \
	-fsanitize=address,undefined

ENV_VARS = ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppr.txt:print_suppressions=false

# On Ubuntu and WSL, googletest is installed to /usr/include or
# /usr/local/include, which are used by default.

# On Mac, we need to manually include them in our path. Brew installs to
# different locations on Intel/Silicon, so ask brew where things live.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	LLVM_PREFIX := $(shell brew --prefix llvm)
	CXX := $(LLVM_PREFIX)/bin/clang++
	CXXFLAGS += -I$(GTEST_PREFIX)/include
	CXXFLAGS += -L$(GTEST_PREFIX)/lib
endif

# Test content won't change, so saving object files helps a lot with
# googletest macro processing time
build/%.o: tests/%.cpp
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.cpp
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

# All public tests: ours and the students'
TEST_NAMES := $(basename $(notdir $(wildcard tests/*.cpp))) canvaslist_tests
TEST_OBJS := $(addprefix build/,$(addsuffix .o,$(TEST_NAMES)))

canvaslist_tests: $(TEST_OBJS) build/shape.o build/canvaslist.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgmock -lgtest_main -o $@

test_shapes: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="ShapeClass*:CircleClass*:RectClass*:RightTriangleClass*"

test_core: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CanvasListCore*"

test_iterating: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CanvasListIterating*"

test_modifying: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CanvasListModifying*"

test_extras: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="CanvasListExtras*"

test_all: canvaslist_tests
	$(ENV_VARS) ./$< --gtest_color=yes

canvaslist_main: build/canvaslist.o build/canvaslist_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

run_canvaslist: canvaslist_main
	$(ENV_VARS) ./$<

clean:
	rm -f canvaslist_tests canvaslist_main build/*
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean test_shapes test_all test_core test_iterating test_modifying test_extras test_all
