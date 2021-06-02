CXX = g++
OBJECTS = ./build/obj/main.o \
					./build/obj/linear-fit.o \
					./build/obj/propagation-uncertainty-calculator.o \
					./build/obj/best.o
OBJ-FLAGS = -c
DEBUG-FLAGS = -g
ROOT-LIBS = `root-config --libs`
ROOT-CFLAGS = `root-config --cflags`
APPNAME = analysis

all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(ROOT-LIBS) -o $(APPNAME)

./build/obj/main.o: ./src/main.cpp \
				./src/linear-fit/linear-fit.h \
				./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h \
				./src/best/best.h
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $< -o ./build/obj/main.o

./build/obj/linear-fit.o: ./src/linear-fit/linear-fit.cpp \
													./src/linear-fit/linear-fit.h
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $< -o ./build/obj/linear-fit.o

./build/obj/propagation-uncertainty-calculator.o: ./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.cpp \
													./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $< -o ./build/obj/propagation-uncertainty-calculator.o

./build/obj/best.o: ./src/best/best.cpp \
										./src/best/best.h
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $< -o ./build/obj/best.o

debug: CXX += $(DEBUG-FLAGS)
debug: $(APPNAME)
	gdb ./$(APPNAME)

clean:
	rm $(OBJECTS)
	rm $(APPNAME)

