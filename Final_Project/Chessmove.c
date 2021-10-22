// bishop logic

int mapArr[8][8] = {
   {-4,-2,-3,-5,-6,-3,-2,-4},
   {-1,-1,-1,-1,-1,-1,-1,-1},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 1, 1, 1, 1, 1, 1, 1, 1},
   { 4, 2, 3, 5, 6, 3, 2, 4}
};

typedef enum{
	EMPTY,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
} chess_pieces;


typedef enum{
	BLACK,
	WHITE
} player_color;


typedef enum{
	SAFE,
	THREATEN
} treaten_check;


typedef enum{
	DEFENDER,
	ATTACKER
}	treater_check;



typedef struct coord{
	chess_pieces *pieces;
	player_color *player;
	int coord_x;
	int coord_y;
	treaten_check *treaten_check;
	treater_check *treater_check;
} xy_coord;
/*
typedef struct{
	player_color* color;
	int turn;
	int* pArr = mapArr[choose_x,choose_y];
	int* kp = mapArr[king_x,king_y];
	int* qp = mapArr[queen_x,queen_y];
	int* rp = mapArr[rook_x,rook_y];;
	int* rp1 = mapArr[rook1_x,rook1_y];;
	int* bp = mapArr[bishop_x,bishop_y];;
	int* bp1 = mapArr[bishop1_x,bishop1_y];;
	int* knp = mapArr[knight_x,knight_y];;
	int* knp1 = mapArr[knight1_x,knight1_y];;
}player_info;
*/

typedef struct {
	int turn;
	int check_state;
	int check_mate_state;
	int threat_state;
	int invalid_state;
	int threat_doubt;
} game_state;



static int i=0;
static int check_mate_state = 0;
static int selected_piece =0;
static int Touch_Pressed=1;
static int Touch_division=0;
static int x_buf1=0,y_buf1 =0;
static int checked = 0;
static int turn =1;


void check_threate_doubt(game_state* state,int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(turn){
		for(i=17;i<34;i++){
			if(pieces_xy_coord[i].coord_x==x_buf1 && pieces_xy_coord[i].coord_y==y_buf1){
				pieces_xy_coord[i].treaten_check = THREATEN;
				state->threat_doubt = 1;
			}
			else{
				state->threat_doubt = 0;
			}
		}
	}
	else{
		i=0;
		for(i;i<17;i++){
			if(pieces_xy_coord[i].coord_x==x_buf1 && pieces_xy_coord[i].coord_y==y_buf1){
				state->threat_doubt = 1;
			}
			else{
				state->threat_doubt = 0;
			}
		}
	}
}

void king_move(game_state *state, int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		int i=0, j=0;
		for(i;i<34;i++){
			if(pieces_xy_coord[i].treater_check == ATTACKER){
				for(j;j<34;j++){
					if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
			
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check  == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
				}
			}
		}
	}
	else{
		//
		//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
	
	}
}

void queen_move(game_state *state, int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		int i=0, j=0;
		for(i;i<34;i++){
			if(pieces_xy_coord[i].treater_check == ATTACKER){
				for(j;j<34;j++){
					if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x )&& (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x )&& (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
		
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + j == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
						if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
		
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else{
						continue;
					}
				}
			}
		}
	}
	else{
		//
		//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
	
	}
}


void rook_move(game_state *state, int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		int i=0, j=0;
		for(i;i<34;i++){
			if(pieces_xy_coord[i].treater_check == ATTACKER){
				for(j;j<34;j++){
					if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x )&& (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x )&& (pieces_xy_coord[i].coord_y == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
		
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + j == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y)){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else{
						continue;
					}
				}
			}
		}
	}
	else{
		//
		//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
	
	}
}

void bishop_move(game_state *state, int** mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		int i=0, j=0;
		for(i;i<34;i++){
			if(pieces_xy_coord[i].treater_check == ATTACKER){
				for(j;j<34;j++){
					if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
		
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + j == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - j == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else{
						continue;
					}
				}
			}
		}
	}
	else{
		//
		//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
	
	}
}


void knight_move(game_state *state, int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		int i=0, j=0;
		for(i;i<34;i++){
			if(pieces_xy_coord[i].treater_check == ATTACKER){
				for(j;j<34;j++){
					if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 2 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 2 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 2 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 2 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){
						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x -2 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 2 == pieces_xy_coord[j].coord_y) ){
						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 2  == pieces_xy_coord[j].coord_y) ){

						//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 2 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){
							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					}
					else{
						continue;
					}
				}
			}
		}
	}
	else{
		//
		//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
	
	}
}


void pawn_move(game_state *state, int **mArr, xy_coord* pieces_xy_coord,int y_buf1,int x_buf1){
	if(state->threat_doubt){
		if(turn){
			int i=0, j=0;
			for(i;i<34;i++){
				if(pieces_xy_coord[i].treater_check == ATTACKER){
					for(j;j<34;j++){
						if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1  == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){

							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else{
						}
					}
				}
			}
		}
		else{
			int i=0, j=0;
			for(i;i<34;i++){
				if(pieces_xy_coord[i].treater_check == ATTACKER){
					for(j;j<34;j++){
						if((pieces_xy_coord[j].treaten_check == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x - 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){

							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else if((pieces_xy_coord[j].treaten_check  == THREATEN) && (pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x + 1 == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){
							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else{
						}
					}
				}
			}
		}
	}
	else{
		if(turn){
			int i=0, j=0;
			for(i;i<34;i++){
				if(pieces_xy_coord[i].treater_check == ATTACKER){
					for(j;j<34;j++){
						if((pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y + 1 == pieces_xy_coord[j].coord_y) ){
							//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else{
						}
					}
				}
			}
		}
		else{
			int i=0, j=0;
			for(i;i<34;i++){
				if(pieces_xy_coord[i].treater_check == ATTACKER){
					for(j;j<34;j++){
						if((pieces_xy_coord[j].coord_x == x_buf1) && (pieces_xy_coord[j].coord_y == y_buf1) && (pieces_xy_coord[i].coord_x == pieces_xy_coord[j].coord_x ) && (pieces_xy_coord[i].coord_y - 1 == pieces_xy_coord[j].coord_y) ){
								//display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
						}
						else{
						}
					}
				}
			}
		}
	}
}

int init_chess(void)
{

	xy_coord* pieces_xy_coord[34] = { 
	{KING,BLACK,0,4,0,0},
	{QUEEN,BLACK,0,3,0,0},
	{ROOK,BLACK,0,0,0,0},
	{BISHOP,BLACK,0,2,0,0},
	{KNIGHT,BLACK,0,1,0,0},
	{ROOK,BLACK,0,7,0,0},
	{BISHOP,BLACK,0,5,0,0},
	{KNIGHT,BLACK,0,6,0},
	{PAWN,BLACK,1,0,0,0},
	{PAWN,BLACK,1,1,0,0},
	{PAWN,BLACK,1,2,0,0},
	{PAWN,BLACK,1,3,0,0},
	{PAWN,BLACK,1,4,0,0},
	{PAWN,BLACK,1,5,0,0},
	{PAWN,BLACK,1,6,0,0},
	{PAWN,BLACK,1,7,0,0},
	{KING,WHITE,7,4.0,0},
	{QUEEN,WHITE,7,3,0,0},
	{ROOK,WHITE,7,0,0,0},
	{BISHOP,WHITE,7,2,0,0},
	{KNIGHT,WHITE,7,1,0,0},
	{ROOK,WHITE,7,7,0,0},
	{BISHOP,WHITE,7,5,0,0},
	{KNIGHT,WHITE,7,6,0,0},
	{PAWN,WHITE,6,0,0,0},
	{PAWN,WHITE,6,1,0,0},
	{PAWN,WHITE,6,2.0,0},
	{PAWN,WHITE,6,3.0,0},
	{PAWN,WHITE,6,4,0,0},
	{PAWN,WHITE,6,5,0,0},
	{PAWN,WHITE,6,6,0,0},
	{PAWN,WHITE,6,7,0,0},
	{KING,WHITE,6,4,0,0}
	};


	//game_state state;

	while(1)
	{
		if(Touch_Pressed)
		{
			while(Touch_division==0)
			{
				if(!checked)//piece is not selected
       	{				
       		selected_piece=mapArr[y_buf1][x_buf1];
       	}
      }
     }
   }
	return 0;
}