#include "item.h"

#include "readcsv.h"

const char* item::name(){
    switch (type) {
    case PLANET:
        return readcsv("planet.csv", val, 0);
    }
    return "ERROR";
}

const char* item::description(){
    switch (type) {
    case PLANET:
        return readcsv("planet.csv", val, 1);
    }
    return "ERROR";
}