/************************
 * recursion		*
 * result could be big 	*
 * if conditions	*
 ************************/

#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable


int absv(int x){
	if( x < 0 ) return -x;
	return x;
}

int ismeet(int r1, int c1, int r2, int c2){
	if( r1 == r2 || c1 == c2 ) return 1;
	if( absv(r1 - r2) == absv(c1 -c2) ) return 1;
	return 0;
}

int conflict( __local int* pos, int row, int col ){
	int i;
	for(i=0; i<row; i++){
		if( ismeet( i, pos[i], row, col ) ){
			return 1;
		}
	}
	return 0;
}

//__kernel void nqueen( __global int *num_queen_ptr, __local int *pos ){
__kernel void nqueen( __global int *num_queen_ptr, __local int *pos, __global int *mutex /*__local char *__print_buf*/ ){
	int row, col, i;
	int queen_placed;
	int global_id = get_global_id(0);
	int num_queen = *num_queen_ptr;

	for( i=0; i<num_queen; i++){
		pos[i] = -1;
	}

	//row = 0;
	row = 1;
	pos[0] = global_id;
	//printf( "id = %d\n", pos[0] );
	//while( row >= 0 ){
	while(row >= 1){
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
		if( queen_placed && row == num_queen - 1 ){ 	//a solution found
			while( *mutex != global_id ){
				atomic_cmpxchg( mutex, -1, global_id );
			}
			for(i=0; i<num_queen; i++){
				printf("(%d, %d) ", i, pos[i] );
			}
			printf("\n");
			atomic_xchg( mutex, -1 );
			continue;
		}
		row++;
	}
}
