CXX = g++
OBJECTS = ./build/obj/main.o \
					./build/obj/linear-fit.o \
					./build/obj/propagation-uncertainty-calculator.o
OBJ-FLAGS = -c
DEBUG-FLAGS = -g
ROOT-LIBS = `root-config --libs`
ROOT-CFLAGS = `root-config --cflags`
SYMBCXX-FLAGS = -lsymbolicc++ -fno-elide-constructors 
SYMBCXX-H = -I $(PWD)/SymbolicC++/SymbolicC++3/headers/ 
SYMBCXX-L = -L $(PWD)/SymbolicC++/SymbolicC++3-3.35/.libs/
APPNAME = analysis

all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CXX) $(SYMBCXX-FLAGS) $(SYMBCXX-L) $(OBJECTS) $(ROOT-LIBS) -o $(APPNAME)

./build/obj/main.o: ./src/main.cpp \
				./src/linear-fit/linear-fit.h \
				./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h 
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $(SYMBCXX-FLAGS) $< -o ./build/obj/main.o

./build/obj/linear-fit.o: ./src/linear-fit/linear-fit.cpp \
													./src/linear-fit/linear-fit.h
	$(CXX) $(OBJ-FLAGS) $(ROOT-CFLAGS) $< -o ./build/obj/linear-fit.o

./build/obj/propagation-uncertainty-calculator.o: ./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.cpp \
													./src/propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h
	$(CXX) $(SYMBCXX-H) $(SYMBCXX-L) $(OBJ-FLAGS) $(ROOT-CFLAGS) $(SYMBCXX-FLAGS) $< -o ./build/obj/propagation-uncertainty-calculator.o

debug: CXX += $(DEBUG-FLAGS)
debug: $(APPNAME)
	gdb ./$(APPNAME)

env: $(APPNAME)
	export LD_LIBRARY_PATH=$(PWD)/SymbolicC++/SymbolicC++3-3.35/.libs/

clean:
	rm $(OBJECTS)
	rm $(APPNAME)

