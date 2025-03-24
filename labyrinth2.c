#include <stdio.h>
#include <stdlib.h>

#define OFF_EDGE -1
#define DEAD_END -2
#define WRONG_COLOUR -3
#define GOAL 100
#define TOTAL_ROUTES 6+5+6+5
#define NUMPERMS 9*8*7*6*5*4*3*2

#define BLACK 1
#define BLUE 2
#define RED 3
// CiSRA 2012 1.4 Labyrinth


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
	t_pos input;
	t_pos output;
	char colour; //usually 0, nonzero if there's a man there
	} t_tileroute;

typedef struct {
	int tilenum; //inherent tile name
	t_tileroute tileroute[TOTAL_ROUTES];
	int tilepos; //current position in permutation
	} t_tile;
	
	// t_tile  tiles[9] = {1,
		// {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2},
		// 44};
	
	

static t_tile  tiles[9] = {
	{	1,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'X',0},BLACK}, //X,0 = dead end
		{{'L',3,},{'X',0},BLACK},
		{{'L',4,},{'B',2},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'T',4},0},
		{{'R',3,},{'R',4},0},
		{{'R',4,},{'R',3},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'T',3},0},
		{{'T',3,},{'T',2},0},
		{{'T',4,},{'R',2},0},
		{{'T',5,},{'X',0},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',5},0},
		{{'B',2,},{'L',4},0},
		{{'B',3,},{'B',5},0},
		{{'B',4,},{'X',0},0},
		{{'B',5,},{'B',3},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	2,
		{	
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'T',2},0}, 
		{{'L',3,},{'L',4},0},
		{{'L',4,},{'L',3},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'T',5},0},
		{{'R',3,},{'T',4},0},
		{{'R',4,},{'B',5},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'L',2},0},
		{{'T',3,},{'B',2},0},
		{{'T',4,},{'R',3},0},
		{{'T',5,},{'R',2},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',5},0},
		{{'B',2,},{'T',3},0},
		{{'B',3,},{'B',4},0},
		{{'B',4,},{'B',3},0},
		{{'B',5,},{'R',4},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	3,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'T',2},0}, 
		{{'L',3,},{'L',4},0},
		{{'L',4,},{'L',3},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'T',5},0},
		{{'R',3,},{'X',0},BLUE},
		{{'R',4,},{'X',0},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'L',2},0},
		{{'T',3,},{'B',3},0},
		{{'T',4,},{'X',0},0},
		{{'T',5,},{'R',2},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'X',0},0},
		{{'B',3,},{'T',3},0},
		{{'B',4,},{'B',5},0},
		{{'B',5,},{'B',4},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	4,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'T',4},0}, 
		{{'L',3,},{'X',0},RED},
		{{'L',4,},{'B',3},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'B',4},0},
		{{'R',3,},{'R',4},0},
		{{'R',4,},{'R',3},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'X',0},0},
		{{'T',3,},{'X',0},0},
		{{'T',4,},{'L',2},0},
		{{'T',5,},{'X',0},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'X',0},0},
		{{'B',3,},{'L',4},0},
		{{'B',4,},{'R',2},0},
		{{'B',5,},{'X',0},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	5,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'X',0},0}, 
		{{'L',3,},{'B',2},0},
		{{'L',4,},{'X',0},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'T',5},0},
		{{'R',3,},{'X',0},BLUE},
		{{'R',4,},{'T',4},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'T',3},0},
		{{'T',3,},{'T',2},0},
		{{'T',4,},{'R',4},0},
		{{'T',5,},{'R',2},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'L',3},0},
		{{'B',3,},{'X',0},0},
		{{'B',4,},{'B',5},0},
		{{'B',5,},{'B',4},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	6,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'X',0},0}, 
		{{'L',3,},{'T',2},0},
		{{'L',4,},{'B',2},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'X',0},0},
		{{'R',3,},{'B',5},0},
		{{'R',4,},{'X',0},BLACK},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'L',3},0},
		{{'T',3,},{'B',4},0},
		{{'T',4,},{'T',5},0},
		{{'T',5,},{'T',4},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'L',4},0},
		{{'B',3,},{'X',0},BLACK},
		{{'B',4,},{'T',3},0},
		{{'B',5,},{'R',3},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	7,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'X',0},0}, 
		{{'L',3,},{'B',2},0},
		{{'L',4,},{'X',0},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'R',3},0},
		{{'R',3,},{'R',2},0},
		{{'R',4,},{'B',4},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'X',0},BLACK},
		{{'T',3,},{'T',4},0},
		{{'T',4,},{'T',3},0},
		{{'T',5,},{'X',0},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'L',3},0},
		{{'B',3,},{'X',0},0},
		{{'B',4,},{'R',4},0},
		{{'B',5,},{'X',0},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	8,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'X',0},0}, 
		{{'L',3,},{'T',2},0},
		{{'L',4,},{'B',2},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'R',3},0},
		{{'R',3,},{'R',2},0},
		{{'R',4,},{'T',5},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'L',3},0},
		{{'T',3,},{'X',0},0},
		{{'T',4,},{'B',3},0},
		{{'T',5,},{'R',4},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'L',4},0},
		{{'B',3,},{'T',4},0},
		{{'B',4,},{'B',5},0},
		{{'B',5,},{'B',4},0},
		{{'B',6,},{'R',5},0}
		}, 0
	},
	{	9,
		{		
		{{'L',1,},{'T',1},0},
		{{'L',2,},{'L',3},0}, //X,0 = dead end
		{{'L',3,},{'L',2},0},
		{{'L',4,},{'X',0},0},
		{{'L',5,},{'B',1},0},
		{{'R',1,},{'T',6},0},
		{{'R',2,},{'X',0},0},
		{{'R',3,},{'R',4},0},
		{{'R',4,},{'R',3},0},
		{{'R',5,},{'B',6},0},
		{{'T',1,},{'L',1},0},
		{{'T',2,},{'B',2},0},
		{{'T',3,},{'B',4},0},
		{{'T',4,},{'X',0},0},
		{{'T',5,},{'B',5},0},
		{{'T',6,},{'R',1},0},
		{{'B',1,},{'L',6},0},
		{{'B',2,},{'T',2},0},
		{{'B',3,},{'X',0},0},
		{{'B',4,},{'T',3},0},
		{{'B',5,},{'T',5},0},
		{{'B',6,},{'R',5},0}
		}, 0
	}
	
	
};


t_tile * pTiles = tiles; //pointer to the beginning of the struct
	

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
		if (pTiles[i].tilepos == pos) return &(pTiles[i]);
	}
	return NULL;
}
	

// Following through: 
//Take input pos, retrieve output pos, see if there's a tile there
// If not, end; if so, work out which side and pos we're entering
int fnFollow(t_tile * oldtile, t_pos * startoutpos, int badcolour) 
{
	t_tile * newtile;
	char new_in_side;
	int i;
	int ret = 0;
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
		entrance_being_checked = &(newtile->tileroute[i].input);
		if (entrance_being_checked->rowcol != startoutpos->rowcol || entrance_being_checked->side != new_in_side) {
			continue; //not the right route on this tile, keep looking at this tile's routes
		}
		//the side and row/col match
		//is it a dead end, a goal, or an exit?
		if (newtile->tileroute[i].colour > 0) {
			//printf("Reached tile number %d, entering %c side row/col %d, found a man of colour %d.\n",newtile->tilenum, newtile->tileroute[i].input.side, newtile->tileroute[i].input.rowcol, newtile->tileroute[i].colour);
			if (newtile->tileroute[i].colour == badcolour) ret = WRONG_COLOUR;
			else ret = GOAL;
			break;
		}
		else if (newtile->tileroute[i].output.side == 'X') {
			ret = DEAD_END;
			break; //found the right entrance, but it was a dead end - give up on this tile
		}		
		else {
			ret = fnFollow(newtile, &(newtile->tileroute[i].output), badcolour); //carry on tracing the route!
			if (ret == DEAD_END || ret == OFF_EDGE || ret == GOAL || ret == WRONG_COLOUR) break;
		}
	}
	return ret;
}

void swap(char * i, char * j)
{
	char swap;
    swap = *i; 
	*i = *j; 
	*j = swap; //more readable than xor
}
	

void reverse(char * string, char * end)
{
	while (end > string) {
		swap (string,end);
		string++; end--;
	}
	return;
}



char currentPerm[9] = "123456789";

int fnGetNextPerm() {
//Based on http://stackoverflow.com/questions/352203/generating-permutations-lazily

//currentPerm is a global pointer to the global 9-char array which was initialised to "123456789"
//TODO this seems to be swapping 7 and 8 first, rather than 8 and 9 - fix off-by-one error
//and it's only getting about 1/30th of the perms it should = probably same bug
	char * end = currentPerm + 8; //pointer to end digit
	char * i = end;
	int n;
	int ret;
	
	
    for(;;) {
        char * ii = i--;
        if (*i <*ii) {
                char * j = end + 1;
                while (!(*i <*--j));
				swap (i,j);
            
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
		pTiles[currentPerm[n] - 48 - 1].tilepos = n + 1; //char to int
	}
	return ret;

	
}

fnTry(char side, int rowcol, int tilenum, int badcolour) { //tilenum is the actual name written on it
	t_pos startpos;
	int ret;
	startpos.side = side;
	startpos.rowcol = rowcol;
	ret = fnFollow(&(pTiles[tilenum-1]), &startpos, badcolour); //tile named tilenum
	//if (ret == WRONG_COLOUR) printf ("Found a route, but colours are wrong\n");
	}

	
	
// For each permutation of 9 tiles
	// For each starting point of red, blue1, blue2
		// Follow it through until you get a dead end (next), or go off the edge (next), or hit another colour (print some data)
int main(){
	int nPermsTried = 0;
	
	printf("Starting:\n");
		
	while(fnGetNextPerm()){
		// Hardcode tile and position of each starting point
		nPermsTried++;
		//printf("Trying permutation %s\n",currentPerm);
		
		//start with red
		if (0 > fnTry('L',3,4,BLUE)) continue; //if dead end or off edge, give up on this perm
		//but if goal, try the other start points in this perm
		
		if (0 > fnTry('R',3,3,RED)) continue; //blue1
		if (0 > fnTry('R',3,5,RED)) continue;  //blue2
		
		if (0 > fnTry('L',2,1,0)) continue;	//black1	
		if (0 > fnTry('L',3,1,0)) continue;	//black2
		if (0 > fnTry('B',3,6,0)) continue;	//black3
		if (0 > fnTry('R',4,6,0)) continue;	//black4
		if (0 > fnTry('T',2,7,0)) continue;	//black5
		
		
		printf("All correct routes found for permutation %s\n", currentPerm);

	}
	printf ("Perms tried: %d\n",nPermsTried);
	return 0;
}

		

	


