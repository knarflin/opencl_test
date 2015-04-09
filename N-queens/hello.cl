
__kernel void hello( __local char* input ){

//	printf( "work_dim = %2d\n", get_work_dim() );	//1
//	printf( "global_id = %2d\n", get_global_id(0) ); //V
//	printf( "group_id = %2d\n", get_group_id(0) );	//V
//	printf( "local_id = %2d\n", get_local_id(0) );	//0
//	printf( "global_size = %2d\n", get_global_size(0) );	//11
//	printf( "local_size = %2d\n", get_local_size(0) );	//1
//	printf( "num_groups = %2d\n", get_num_groups(0) );	//11
	
	//int num_queen = input[0];
	//int pos[8] = {0};
	int x = get_global_id(0);
	int y = get_local_id(0);
	int z = get_group_id(0);

	
	input[x]=x*10;
	if(	(input[0] ==  0) &&
		(input[1] == 10) &&
		(input[2] == 20) &&
		(input[3] == 30) &&
		(input[4] == 40) &&
		(input[5] == 50) )
		printf("%s", input);
	printf("id = %d, %d, %d ", x, y, z);
	printf("%p, =%2d ", &input[0], input[0] ); //debug point
	printf("%p, =%2d ", &input[1], input[1] ); //debug point
	printf("%p, =%2d ", &input[2], input[2] ); //debug point
	printf("%p, =%2d ", &input[3], input[3] ); //debug point
	printf("%p, =%2d ", &input[4], input[4] ); //debug point
	printf("%p, =%2d\n", &input[5], input[5] ); //debug point
	//func( pos, 1, 8 );
}
