CXX=g++
SRC=$(shell ls *.cpp)
OBJ=$(SRC:%.cpp=obj/%.o)
DEP= $(OBJ:.o=.d)
ODIR=obj

jokerpoker: $(OBJ) | $(ODIR)
	$(CXX) -o $@ $^ -lncursesw

$(ODIR): #create obj directory if it doesn't exist
	mkdir $(ODIR)

obj/%.h: %.csv enummaker.sh
	./enummaker.sh $< $@

$(ODIR)/%.d: %.cpp
	$(CXX) -MM -MT $(@:.d=.o) -MG $< >> $@

include $(DEP)

$(ODIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< -lncursesw

run: jokerpoker
	./jokerpoker