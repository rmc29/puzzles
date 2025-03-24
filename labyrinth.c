#include <stdio.h>
#include <stdlib.h>

#define OFF_EDGE -1
#define DEAD_END -2
#define GOAL 100
#define TOTAL_ROUTES 6+5+6+5
#define NUMPERMS 9*8*7*6*5*4*3*2

#define BLACK 1
#define BLUE 2
#define RED 3
// CiSRA 2012 1.4 Labyrinth

//TODO: This compiles but would be v tedious to malloc and fill all the structs, so try making them not pointers and initialising them directly. Would have to make the NULL check for dead ends instead a self-equal check.
//Or just have one big static super-struct , and pass around pointers within it, no malloc.

// Tile data structure:
	// Name
	// 6+6+5+5 (input,output) pairs

// Pair:
	// Input (pos)
	// Output (pos)
	
// Pos:
	// Side
	// Row/col

typedef struct {
	char side;
	int rowcol;
	} t_pos;
	
typedef struct {
	t_pos * input;
	t_pos * output;
	char colour; //usually 0, nonzero if there's a man there
	} t_tileroute;

typedef struct {
	int tilenum; //inherent tile name
	t_tileroute tileroute[TOTAL_ROUTES];
	int tilepos; //current position in permutation
	} t_tile;
	

t_tile * tiles[9];
	


//Tile data is hardcoded here	
 void fnPopulateTiles(void) {
	int i,j;
	for (i=0;i<9;i++) {
		tiles[i] = malloc(sizeof(*tiles));
		tiles[i]->tilenum = i+1;
		for (j=0;j<TOTAL_ROUTES;j++)
		{
			tiles[i]->tileroute[j].input = calloc(sizeof(t_pos),0);
			tiles[i]->tileroute[j].output = calloc(sizeof(t_pos),0);
		}
	
	tiles[i]->tileroute[0].input->side = 'L';
	tiles[i]->tileroute[0].input->rowcol = 1;
	tiles[i]->tileroute[0].output->side = 'T';
	tiles[i]->tileroute[0].output->rowcol = 1;
	
	tiles[i]->tileroute[1] = (t_tileroute) {{'L',2,},NULL,BLACK},
	// {'L',3,},NULL,BLACK},
	// {'L',4,},{'B',2},0},
	// {'L',5,},{'B',1},0},
	// {'R',1,},{'T',6},0},
	// {'R',2,},{'T',4},0},
	// {'R',3,},{'R',4},0},
	// {'R',4,},{'R',3},0},
	// {'R',5,},{'B',6},0},
	// {'T',1,},{'L',1},0},
	// {'T',2,},{'T',3},0},
	// {'T',3,},{'T',2},0},
	// {'T',4,},{'R',2},0},
	// {'T',5,},NULL,0},
	// {'T',6,},{'R',1},0},
	// {'B',1,},{'L',6},0},
	// {'B',2,},{'L',5},0},
	// {'B',3,},{'B',5},0},
	// {'B',4,},NULL,0},
	// {'B',5,},{'B',3,},0},
	// {'B',6,},{'R',5},0}}; //for testing we make all 9 tiles a copy of #1; afterwards take this out of the for loop
	// }
						   	
}	
	
 }


char fnOpposite(char side)
{	
	if (side == 'L') return 'R';
	if (side == 'R') return 'L';
	if (side == 'T') return 'B';
	if (side == 'B') return 'T';
}

int fnGetAdjacentTilePos(t_tile * tile, t_pos * startoutpos)
{
	int adjacentTilePos = tile->tilepos;
	switch (startoutpos->side){
	case 'L':
		if (tile->tilepos == 1 || tile->tilepos == 4 || tile->tilepos == 7) adjacentTilePos = OFF_EDGE;
		else adjacentTilePos--;
		break;
	case 'R':
		if (tile->tilepos == 3 || tile->tilepos == 6 || tile->tilepos == 9) adjacentTilePos = OFF_EDGE;
		else adjacentTilePos++;
		break;
	case 'T':
		if (tile->tilepos == 1 || tile->tilepos == 2 || tile->tilepos == 3) adjacentTilePos = OFF_EDGE;
		else adjacentTilePos-=3;
		break;
	case 'B':
		if (tile->tilepos == 7 || tile->tilepos == 8 || tile->tilepos == 9) adjacentTilePos = OFF_EDGE;
		else adjacentTilePos+=3;
		break;
	}
	return adjacentTilePos;
}

t_tile * fnFindTileByPos(int pos)
{	
	int i;
	for (i=0;i<9;i++)
	{
		if (tiles[i]->tilepos == pos) return tiles[i];
	}
	return NULL;
}
	

// Following through: 
//Take input pos, retrieve output pos, see if there's a tile there
// If not, end; if so, work out which side and pos we're entering
int fnFollow(t_tile * oldtile, t_pos * startoutpos) 
{
	t_tile * newtile;
	char new_in_side;
	int i;
	int ret;
	t_pos * entrance_being_checked;
	//starting with something like: tile number 1 (which is in pos 3), out at left row 4
	//need the tile that is to its (e.g.) right, if there is one
	int adjacentTilePos = fnGetAdjacentTilePos(oldtile, startoutpos);
	if (0 > adjacentTilePos) return OFF_EDGE;
	//now we know we're looking at, say, the tile in position 2
	//dig up which tile number that is
	newtile = fnFindTileByPos(adjacentTilePos);
	
	//want to find the route from the RIGHT row 4 entrance
	//iterate over the tile's routes until you find R4
	new_in_side = fnOpposite(startoutpos->side);
	for (i=0;i<TOTAL_ROUTES;i++)
	{	
		entrance_being_checked = newtile->tileroute[i].input;
		if (entrance_being_checked->rowcol != startoutpos->rowcol || entrance_being_checked->side != new_in_side) {
			continue; //not the right route on this tile, keep looking at this tile's routes
		}
		//the side and row/col match
		//is it a dead end, a goal, or an exit?
		if (newtile->tileroute[i].colour > 0) {
			printf("Reached tile number %n, entering %c side row/col %n, found a man of colour %n.\n",newtile->tilenum, newtile->tileroute[i].input->side, newtile->tileroute[i].input->rowcol, newtile->tileroute[i].colour);
			ret = GOAL;
			break;
		}
		else if (newtile->tileroute[i].output == NULL) {
			ret = DEAD_END;
			break; //found the right entrance, but it was a dead end - give up on this tile
		}		
		else fnFollow(newtile, newtile->tileroute[i].output); //carry on tracing the route!
	}
	return ret;
}

void reverse(char * string, char * end)
{
	char swap;
	while (end > string) {
		swap = *string; *string = *end; *end = swap;
		string++; end--;
	}
	return;
}
	

char* currentPerm = "123456789";

int fnGetNextPerm() {
//Based on http://stackoverflow.com/questions/352203/generating-permutations-lazily

//currentPerm is a global pointer to the global 9-char array which was initialised to "123456789"
	char * end = currentPerm + 8; //pointer to end digit
	char * i = end;
	char swap;
	int n;
	int ret;
    --i;
	
	//for testing, let's have this return false, so we only ever test one permutation
	return 0;
	
    for(;;) {
        char * ii = i--;
        if (*i <*ii) {
                char * j = end;
                while (!(*i <*--j));
                swap = *i; *i = *j; *j = swap; //more readable than xor
                reverse(ii, end);
                ret = 1;
				break;
        }
        if (i == currentPerm) {
                reverse(currentPerm, end);
                ret = 0;
				break;
        }
    }
	
	for (n=0;n<9;n++) {
		tiles[n]->tilepos = currentPerm[n];
	}
	return ret;

	
}

	
	
// For each permutation of 9 tiles
	// For each starting point of red, blue1, blue2
		// Follow it through until you get a dead end (next), or go off the edge (next), or hit another colour (print some data)
int main(){

	
	fnPopulateTiles();
	
	while(fnGetNextPerm()){
		// Hardcode tile and position of each starting point
		//start with red
		int result;
		t_pos * startpos;
		startpos->side = 'L';
		startpos->rowcol = 3;
		result = fnFollow(fnFindTileByPos(4), startpos);
		if (result == DEAD_END) {
			//give up on this perm
			continue;
		} 
			//if dead end, next permutation
			//if goal, try the other start points in this perm
		
		startpos->side = 'R';
		startpos->rowcol = 3;
		result = fnFollow(fnFindTileByPos(3), startpos);
		if (result == DEAD_END) {
			//give up on this perm
			continue;
		}
		startpos->side = 'R';
		startpos->rowcol = 3;
		result = fnFollow(fnFindTileByPos(5), startpos);
		if (result == DEAD_END) {
			//give up on this perm
			continue;
		}
	}
		
}

		

	


