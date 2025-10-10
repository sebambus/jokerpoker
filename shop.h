#pragma once

class game;

class shop{
public:
    game *g;

    shop(game *g);
    void enter();
};