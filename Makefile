TARGET=test
CPPFLAGS = -Wall -std=c++11
OBJS = test.o

$(TARGET) : $(OBJS)
	g++ $^ -o $@

test.o : skip_list.hpp skip_list_node.hpp

.PHONY:
clean:
	rm $(OBJS) $(TARGET)
