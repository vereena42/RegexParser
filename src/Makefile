TARGET = reg_mo
OBJS = reg_mo.o

CXXFLAGS = -std=c++11

all = $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	
clean:
	$(RM) *.o *.a $(TARGET)

.PHONY: all clean
