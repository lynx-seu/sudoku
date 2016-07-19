#general
CC = g++
RM = rm -rf
CPPFLAGS = -g -O2 -std=c++11 

# == END OF USER SETTINGS. NO NEED TO CHANGE ANYTHING BELOW THIS LINE =========
TARGET = main 
ALL_O = main.o dlx.o sudoku.o

all: $(TARGET)

o: $(ALL_O)

$(TARGET): $(ALL_O)
	$(CC)  $^ $(LDFLAGS) -o $@

clean:
	$(RM) $(TARGET) $(ALL_O)


# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all $(PLATS) default o a clean

# DO NOT DELETE
main.o: main.cpp
dlx.o:  dlx.cpp
