#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int absv(int x){
	if( x < 0 ) return -x;
	return x;
}

int ismeet(int r1, int c1, int r2, int c2){
	if( r1 == r2 || c1 == c2 ) return 1;
	if( absv(r1 - r2) == absv(c1 -c2) ) return 1;
	return 0;
}

int conflict( int* pos, int row, int col ){
	int i;
	for(i=0; i<row; i++){
		if( ismeet( i, pos[i], row, col ) ){
			return 1;
		}
	}
	return 0;
}

int main( void ){
	int row, col, i;
	int num_queen;
	int queen_placed;
	int* pos = NULL;

	printf("num_queen = ");
	scanf( "%d", &num_queen );
	
	pos = (int*)malloc( sizeof(int) * num_queen );
	if( pos == NULL ){ 
		printf("line %d error\n", __LINE__ ); 
		exit(0);
	}
	for( i=0; i<num_queen; i++){
		pos[i] = -1;
	}


	row = 0;
	while( row >= 0 ){
		queen_placed = 0;
		for( col=pos[row]+1; col<num_queen; col++){
			if( !conflict( pos, row, col ) ){
				pos[row] = col;
				queen_placed = 1;
				break;
			}
		}
		if( !queen_placed ){ 	//no suitable place for queen.
			pos[row] = -1;	//clean the position of current row
			row--;		//back to previous row.
			continue;
		}
		if( queen_placed && row == num_queen - 1 ){ //a solution found
			for(i=0; i<num_queen; i++){
				printf("(%d, %d) ", i, pos[i] );
			}
			printf("\n");
			continue;
		}
		row++;
	}
	return 0;
}
