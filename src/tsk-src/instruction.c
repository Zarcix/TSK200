#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "instruction.h"

DirectionalLocation opposite_of_directional(DirectionalLocation originalDirection) {
    switch (originalDirection) {
        case LEFT: {
            return RIGHT;
        }
        case RIGHT: {
            return LEFT;
        }
        case UP: {
            return DOWN;
        }
        case DOWN: {
            return UP;
        }
        default: {
            return originalDirection;
        }
    }
}