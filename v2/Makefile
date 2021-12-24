CXX = g++
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
INCLUDE =
LIB =
EXEC = Demineur

all : $(EXEC)

%.o: %.cpp
	$(CXX) $(INCLUDE) -c $<

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LIB)

clean:
	del -rf $(OBJ)
	del -rf $(EXEC).exe