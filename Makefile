src = $(wildcard meshworld.cpp)
obj = $(src:.cpp=.o)


LIBRARY_PATHS = -Llib/libst/lib

OS = $(shell uname)
ifeq ($(OS),Darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -lst
else
	LDFLAGS = -lGL -lGLEW -lglut -lGLU -lglfw -lglee -lst
	CXXFLAGS = -std=c++11
endif

meshworld: $(obj)
	cd lib/libst && make
	g++ -g -o $@ $^ $(LIBRARY_PATHS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) meshworld


