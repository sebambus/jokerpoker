CXX=g++
SRC=$(shell ls *.cpp)
OBJ=$(SRC:%.cpp=obj/%.o)
DEP= $(OBJ:.o=.d)
ODIR=obj

jokerpoker: $(OBJ) | $(ODIR)
	$(CXX) -o $@ $^ -lncursesw

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.d: %.cpp
	@$(CXX) -MM -MT $(@:.d=.o) $< > $@

include $(DEP)

$(ODIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< -lncursesw

run: jokerpoker
	./jokerpoker