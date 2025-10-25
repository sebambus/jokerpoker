#include "consumable.h"

#include "readcsv.h"

const char* itemName(item t){
    switch (t.type) {
    case PLANET:
        return readcsv("planet.csv", t.val, 0);
    }
    return "ERROR";
}

const char* itemDescription(item t){
    switch (t.type) {
    case PLANET:
        return readcsv("planet.csv", t.val, 1);
    }
    return "ERROR";
}