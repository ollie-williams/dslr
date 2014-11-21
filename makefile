SOURCES = main.cpp dslrException.cpp camera.cpp camera_list.cpp
OBJS = $(SOURCES:.cpp=.o)

all: dslr

dslr: $(OBJS)
	clang++ $(OBJS) -m32 -F /Library/Frameworks/ -framework EDSDK -framework CoreFoundation -o $@ 

%.o: %.cpp
	clang++ -c -Wall -m32 -o $@ $<

depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	clang++ -MM $^ -MF  ./.depend;

include .depend

clean:
	rm $(OBJS) dslr
