#include "model.h"
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor initializes the object
Model::Model(int h, int w) {
    ended = false;
    height = h;
    width = w;
    score = 0;
    shape = (Tetrominoe)(time(0)%7);
    // for later checks
    blockLocation.x=0;
    blockLocation.y=0;
    // making a boolean grid to check where blocks are
    for (int i=0; i<=height; i++) {
        for (int j=0; j<width; j++) {
            if (i==height) {
                grid[i][j] = true;
            } else {
                grid[i][j] = false;
                colorGrid[i][j] = D;
            }
        }
    }
	spawn();
}
// Destructor deletes dynamically allocated memory
Model::~Model() {
}

bool Model::gameOver() {
    for (int j=0; j<10; j++) {
        if (grid[0][j]) {
            return true;
        }
    }
    return false;
}

void Model::checkRows() {
    bool complete=true;
    for (int i = 0; i<height; i++) {
        for (int j=0; j<width; j++) {
            if (!grid[i][j]) {
                complete=false;
                break;
            }
        }
        if (!complete) {
            complete = true;
        } else {
            deleteRow(i);
        }
    }
}

void Model::deleteRow(int row) {
    for (int j=0; j<width; j++) {
        colorGrid[row][j] = D;
    }
    for (int i=row; i>=1; i--) {
        for (int j=0; j<width; j++) {
            colorGrid[i][j] = colorGrid[i-1][j];
            grid[i][j] = grid[i-1][j];
        }
    }
    for (int j=0; j<width; j++) {
        colorGrid[0][j] = D;
        grid[0][j] = false;
    }
    score++;
}

void Model::spawn() {
    // Create a new piece
    shape = (Tetrominoe)(time(0)%7);
	orientation = UP;
	location.x = 4;
	location.y = 0;
}

Coordinate * Model::block() {
	// All blocks by orientation and shape
	// block[orientation][shape]
	// Treat as a constant
	static Coordinate blocks[4][7][4] = {
	{ // UP
		{ {1, 1}, {1, 2}, {1, 3}, {1, 4} }, // I
		{ {0, 0}, {1, 0}, {0, 1}, {1, 1} }, // O
		{ {1, 1}, {2, 1}, {3, 1}, {2, 0} }, // T
		{ {1, 1}, {2, 1}, {2, 0}, {3, 0} }, // S
		{ {1, 1}, {2, 1}, {2, 2}, {3, 2} }, // Z
		{ {2, 2}, {3, 2}, {3, 1}, {3, 0} }, // J
		{ {1, 1}, {1, 2}, {1, 3}, {2, 3} }, // L
	},
	
	{ // LEFT
		{ {0, 2}, {1, 2}, {2, 2}, {3, 2} }, // I
		{ {0, 0}, {1, 0}, {0, 1}, {1, 1} }, // O
		{ {2, 0}, {2, 1}, {2, 2}, {3, 1} }, // T
		{ {2, 0}, {2, 1}, {3, 1}, {3, 2} }, // S
		{ {3, 1}, {3, 2}, {2, 2}, {2, 3} }, // Z
		{ {2, 0}, {2, 1}, {3, 1}, {4, 1} }, // J
		{ {2, 2}, {1, 2}, {0, 2}, {0, 3} }, // L
	},
	
	{ // DOWN
		{ {1, 1}, {1, 2}, {1, 3}, {1, 4} }, // I
		{ {0, 0}, {1, 0}, {0, 1}, {1, 1} }, // O
		{ {1, 1}, {2, 1}, {3, 1}, {2, 2} }, // T
		{ {1, 1}, {2, 1}, {2, 0}, {3, 0} }, // S
		{ {1, 1}, {2, 1}, {2, 2}, {3, 2} }, // Z
		{ {4, 0}, {3, 0}, {3, 1}, {3, 2} }, // J
		{ {1, 3}, {1, 2}, {1, 1}, {0, 1} }, // L
	},
	
	{ // RIGHT
		{ {0, 2}, {1, 2}, {2, 2}, {3, 2} }, // I
		{ {0, 0}, {1, 0}, {0, 1}, {1, 1} }, // O
		{ {2, 0}, {2, 1}, {2, 2}, {1, 1} }, // T
		{ {2, 0}, {2, 1}, {3, 1}, {3, 2} }, // S
		{ {3, 1}, {3, 2}, {2, 2}, {2, 3} }, // Z
		{ {4, 2}, {4, 1}, {3, 1}, {2, 1} }, // J
		{ {0, 2}, {1, 2}, {2, 2}, {2, 1} }, // L
	},	
	
};
    // Building blocks for Tetrominoes
	return blocks[orientation][shape];
}

// This should build up the pile structure
void Model::build() {
    Coordinate * blck = block();
    for (int i =  0; i < 4; i++) {
        grid[blck[i].y+location.y][blck[i].x+location.x] = true;
        colorGrid[blck[i].y+location.y][blck[i].x+location.x] = shape;
    }
    checkRows();
    spawn();
}

void Model::fall() { // (and do collision detection)
    Coordinate * blck = block();
    for (int i = 0; i < 4; i++) {
        if (grid[blck[i].y+location.y + 1][blck[i].x+location.x]) {
            // yay collision
            build();
            return;
        }
    }
    location.y++;
}

Coordinate Model::right() {
    blockLocation.x = 0;
	Coordinate * blck = block();
	for (int i =  0; i < 4; i++) {
		if (blck[i].x + location.x > blockLocation.x) {
			blockLocation.x = (blck[i].x + location.x);
            blockLocation.y = (blck[i].y + location.y);
		}
	}
	return blockLocation;
}
// we still need to fix that shapes can't go left or right into an earlier shape...
Coordinate Model::left() {
    blockLocation.x = 9;
    Coordinate * blck = block();
    for (int i =  0; i < 4; i++) {
        if (blck[i].x + location.x < blockLocation.x) {
            blockLocation.x = (blck[i].x + location.x);
            blockLocation.y = (blck[i].y + location.y);
        }
    }
    return blockLocation;
}

void Model::go(Direction d) {
    Coordinate * blck = block();
    if (d == LEFT) {
        for (int i = 0; i < 4; i++){
            if (grid[blck[i].y + location.y][blck[i].x+location.x-1]==true)
            {return;}
        }
        if (left().x > 0) {
            location.x--;
        }
    }
    
    if (d == RIGHT) {
        for (int i = 0; i < 4; i++) {
            if (grid[blck[i].y + location.y][blck[i].x+location.x+1] == true) {return;
        }
        }
        if (right().x < 9) {
            location.x++;
        }
    }

    if (d == DOWN) {
		fall();
        fall();
    }
	if (d == UP) {
		orientation = (Direction)((((int)orientation) + 1) % 4);
	}
}