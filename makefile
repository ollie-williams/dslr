SOURCES = main.cpp dslrException.cpp camera.cpp
OBJS = $(SOURCES:.cpp=.o)

all: dslr

dslr: $(OBJS)
	clang++ $(OBJS) -m32 -F /Library/Frameworks/ -framework EDSDK -o $@ 

%.o: %.cpp
	clang++ -c -Wall -m32 -o $@ $<
