#include<iostream>
#include <pthread.h>
//#pragma comment(lib, "pthreadVC2.lib") 

using namespace std;
	
typedef struct {
       int  who ;      
       int** board ;   
       int size;     
       int depth;  
       int max_depth;   
       int alpha;     
		int beta;
		int value;
		pthread_t tid;
}STATE; 

int check_win(int size, int** board, int i, int j)
{
	int who = board[i][j];
	int count1, count2;

	// up + down
	count1 = 0;
	count2 = 0;
	for(int k=1;k<=4;k++){
		if(j+k >= size || board[i][j+k] != who) break;
		count1++;
	}
	for(int k=1;k<=4;k++){
		if(j-k < 0 || board[i][j-k] != who) break;
		count2++;
	}
	if(count1 + count2 >=4) return 1;

	// left + right
	count1 = 0;
	count2 = 0;
	for(int k=1;k<=4;k++){
		if(i+k >= size || board[i+k][j] != who) break;
		count1++;
	}
	for(int k=1;k<=4;k++){
		if(i-k < 0 || board[i-k][j] != who) break;
		count2++;
	}
	if(count1 + count2 >=4) return 1;

	// upleft + downright
	count1 = 0;
	count2 = 0;
	for(int k=1;k<=4;k++){
		if(j+k >= size || i-k < 0 || board[i-k][j+k] != who) break;
		count1++;
	}
	for(int k=1;k<=4;k++){
		if(j-k < 0 || i+k >= size ||  board[i+k][j-k] != who) break;
		count2++;
	}
	if(count1 + count2 >=4) return 1;


	// upright + downleft
	count1 = 0;
	count2 = 0;
	for(int k=1;k<=4;k++){
		if(i+k >= size || j+k>=size || board[i+k][j+k] != who) break;
		count1++;
	}
	for(int k=1;k<=4;k++){
		if(i-k < 0 || j-k<0 || board[i-k][j-k] != who) break;
		count2++;
	}
	if(count1 + count2 >=4) return 1;


	return 0;
}

int nega_max2(int who, int** board, int size, int depth, int max_depth, int alpha, int beta)
{
	int val = alpha;
	int tmp;
	int status;
	// iterate through every child
	for(int i = size -1; i >=0; i--){
		for(int j = 0; j < size; j++){
			if( board[i][j] != 0) continue;	
			board[i][j] = who;

			status = check_win( size, board, i, j);
			if( status == 1 || depth == max_depth-1 ) tmp = status;
			else tmp = -nega_max2( -who, board, size, depth+1, max_depth, -beta, -val );

			board[i][j] = 0;
			if( tmp >= beta ) return tmp;
			if( tmp > val ) val = tmp;
		}
	}
	return val;

}
int nega_max3(int who, int** board, int size, int depth, int max_depth, int alpha, int beta)
{
	int val = alpha;
	int tmp;
	int status;
	// iterate through every child
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if( board[i][j] != 0) continue;	
			board[i][j] = who;

			status = check_win( size, board, i, j);
			if( status == 1 || depth == max_depth-1 ) tmp = status;
			else tmp = -nega_max3( -who, board, size, depth+1, max_depth, -beta, -val );

			board[i][j] = 0;
			if( tmp >= beta ) return tmp;
			if( tmp > val ) val = tmp;
		}
	}
	return val;

}



// NegaMax algorithm
void* nega_max(void* arg)
{
	STATE* p = (STATE*)	arg;
	int val = p->alpha;
	int tmp,tmp_alpha,tmp_beta;
	int status;
	// iterate through every child
	for(int i = 0; i < p->size; i++){
		for(int j = 0; j < p->size; j++){
			if( p->board[i][j] != 0) continue;	
			p->board[i][j] = p->who;

			status = check_win( p->size, p->board, i, j);
			if( status == 1 || p->depth == p->max_depth-1 ) tmp = status;
			else {
				tmp_alpha = p->alpha ;
				tmp_beta = p->beta ;
				p->who = -1 * p->who ;
				p->depth = p->depth + 1 ;
				p->alpha = -1 * p->beta;
				p->beta = -1 * val;
				nega_max( (void*)p );
				tmp = -1 * p->value ;


				p->who = -1 * p->who ;
				p->depth = p->depth - 1 ;
				p->alpha = tmp_alpha;
				p->beta = tmp_beta;
			}
			p->board[i][j] = 0;
			if( tmp >= p->beta ) {
				p->value = tmp;
				return NULL;
			}
			if( tmp > val ) val = tmp;
		}
	}
	p->value = val;
	return NULL;
}



int main()
{

	
	int num_given_white;
	int num_given_black;
	int size;				// board_size, N x N
	int depth_to_search;	// maximum search depth
	int x;
	int y;
	int i,j;
	int** board;			// white = 1, black = -1, empty = 0;

	cin >> size;
	cin >> depth_to_search;
	board = new int*[size];
	for( int i = 0; i < size; i++){
		board[i] = new int[size];
	}
	for( int i = 0; i < size; i++){
		for( int j = 0; j < size; j++){
			board[i][j] = 0; 
		}
	}
	
	cin >> num_given_white;
	for(int i = 0; i < num_given_white; i++){
		cin >> x;
		cin >> y;
		board[x][y] = 1;
	}


	cin >> num_given_black;
	for( int i = 0; i < num_given_black; i++){
		cin >> x;
		cin >> y;
		board[x][y] = -1;
	}
	if(size > 20 || size < 10){
		for( int k = 1; k <= depth_to_search; k++){
			int result;
			result = nega_max2( 1, board, size, 0 , k, -1, 1 );
		
			if( result == -1 ) {
				cout << k << " " << "black";
				break;
			}
			else if( result == 1) {
				cout << k << " " << "white";
				break;
			}
			else if( k == depth_to_search )
				cout << "draw";
		
		}
	}
	else if (size > 10 && size <= 20){
		for( int k = 1; k <= depth_to_search; k++){
			int result;
			result = nega_max3( 1, board, size, 0 , k, -1, 1 );
		
			if( result == -1 ) {
				cout << k << " " << "black";
				break;
			}
			else if( result == 1) {
				cout << k << " " << "white";
				break;
			}
			else if( k == depth_to_search )
				cout << "draw";
		
		}
		
	}
	else{
		int NumberOfThreads = size * size - num_given_white - num_given_black ;
		STATE * thread = new STATE[NumberOfThreads] ;
		for(i = 0 ; i < NumberOfThreads ; i++){
			thread[i].who              =  -1;
			thread[i].size             =  size;
			thread[i].depth            =	1;      
			thread[i].max_depth        =  0;             
			thread[i].alpha            =  -1;
			thread[i].beta             =   1;
			thread[i].value				=  0;
			thread[i].board				=  new int*[size];
			for(j = 0;j < size ; j++){
				thread[i].board[j] =  new int[size];
			}
			for(int k = 0; k < size ; k++){
				for( j = 0; j < size; j++){
					thread[i].board[k][j] = board[k][j];
				
				}
			}
		}

		for( int k = 5; k <= depth_to_search  ; k++){
			int result = -1,temp  ;

			int numThread =0;
			for(int i = 0; i < size; i++){
				for(int j = 0; j < size; j++){
					if( board[i][j] != 0) continue;	
					thread[numThread].board[i][j] = 1;
					thread[numThread].max_depth = k;
					pthread_create (&thread[numThread].tid, NULL, nega_max, &thread[numThread]) ;
					numThread ++ ;
				}
			}

			for(int i = 0 ; i < NumberOfThreads ;i++){
				pthread_join(thread[i].tid , NULL);
				temp = -1 * thread[i].value;
				if( temp >= 1) { result = temp ;break;}
				if( temp > -1 )  result = temp;
			}

			if( result == -1 ) {
				cout << k << " " << "black";
				break;
			}
			else if( result == 1) {
				cout << k << " " << "white";
				break;
			}
			else if( k == depth_to_search )
				cout << "draw";
		

		}
	}
	
	//system("pause");
	return 0;
}
