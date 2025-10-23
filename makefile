CXX=g++
OBJ=obj/card.o obj/color.o obj/deck.o obj/game.o obj/hand.o obj/level.o obj/main.o obj/scorekeep.o obj/shop.o obj/voucher.o obj/window.o
ODIR=obj

#%.d: %.cpp
#	$(CXX) -MM 

$(ODIR)/%.o: %.cpp *.h
	$(CXX) -c -o $@ $< -lncursesw

jokerpoker: $(OBJ)
	$(CXX) -o $@ $^ -lncursesw