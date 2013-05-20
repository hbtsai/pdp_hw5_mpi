#include<iostream>
//#include <omp.h>

#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <memory.h>
#include <pthread.h>

using namespace std;

int size;
int boardOrigin[50][50];			// white = 1, black = -1, empty = 0;
int inputBoard[50][50][50];

int dieCounter;//To count how many thread died.
int whoFinally;
int depthFinally;
bool killAllThread;//if it set to "true", all the other thread will terminate without result

typedef struct passValue{
    int inputI;

}PV;

inline int check_win(int size, int board[][50], int i, int j)
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

int nega_max(int who, int board[][50],int depth, int max_depth, int alpha, int beta);

void *nega_max_parallel(void *t){
    

    int _max_depth;


//    printf("yeah fuckyou=%d\n",_max_depth);
//    int **boardPrivate; // for every thread
    
    int yo,fuck;
//	boardPrivate = new int*[size];
//	for(yo = 0; yo < size; yo++){
//		boardPrivate[yo] = new int[size];
//	}
    
    PV *passValueInput=(PV*)t;
    
//	for( yo = 0; yo < size; yo++){
//		for(fuck = 0; fuck < size; fuck++){
//			boardPrivate[fuck][yo] = boardOrigin[fuck][yo];
//		}
//	}



//    int privateBoard[50][50]=passValueInput->inputBoard;

    
    _max_depth=passValueInput->inputI;
//    cout<<"fuckyou:"<<_max_depth<<endl;
//    cout<<endl;
//    for( int i = 0; i < size; i++){
//		for( int j = 0; j < size; j++){
//            cout<<privateBoard[j][i];
//		}
//        cout<<endl;
//	}
    
    int result = nega_max( 1, inputBoard[_max_depth-1] , 0 , _max_depth, -1, 1 );
    
    if(result==0&&!killAllThread){
        dieCounter++;
//        printf("%d die!\n",_max_depth);
//        pthread_exit(NULL);
    }else if(result==-99){//Being interrupted
//        printf("%d interrupted!\n",_max_depth);
//        pthread_exit(NULL);
    }else if(result!=0){
        killAllThread=true;
        whoFinally=result;
        depthFinally=_max_depth;
//        printf("%d GOT!, depth=%d\n",whoFinally,depthFinally);
//        pthread_exit(NULL);
    }
//    pthread_exit(NULL);
    
//    for(int i = 0; i < size; ++i) {
//        delete [] boardPrivate[i];
//    }
//    delete [] ary;
//    pthread_exit(NULL);
    return 0;
}


int nega_max(int who, int board[][50],int depth, int max_depth, int alpha, int beta)
{
	int val = alpha;
	int tmp;
	int status;
	// iterate through every child
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
//            printf("GO!\n");
            if(killAllThread){//interrupted
                return -99;
            }
			if( board[i][j] != 0) continue;
			board[i][j] = who;
            
			status = check_win( size, board, i, j);
			if( status == 1 || depth == max_depth-1 ) tmp = status;
			else tmp = -nega_max( -who, board, depth+1, max_depth, -beta, -val );
            if(killAllThread){//interrupted
                return -99;
            }
            
			board[i][j] = 0;
			if( tmp >= beta ) return tmp;
			if( tmp > val ) val = tmp;
		}
	}
	return val;
    
}




void printResults(time_t* pt1, time_t* pt2)
{
    double secs;
    int hours , mins, intsecs;
    
    printf("End: \t%s", ctime(pt2));
    secs = difftime(*pt2, *pt1);
    intsecs = (int)secs;
    printf("Calculations took %d second%s.\n", intsecs, (intsecs == 1 ? "" : "s"));
    
    /* Print hours, minutes, seconds */
    hours = intsecs/3600;
    intsecs -= hours * 3600;
    mins = intsecs/60;
    intsecs -= mins * 60;
    if (hours > 0 || mins > 0)
    {
        printf("Equals ");
        if (hours > 0)
        {
            printf("%d hour%s, ", hours, (hours == 1) ? "" : "s");
        }
        if (mins > 0)
        {
            printf("%d minute%s and ", mins, (mins == 1) ? "" : "s");
        }
        printf("%d second%s.\n", intsecs, (intsecs == 1 ? "" : "s"));
        
    }
}




int main()
{
    
	
	int num_given_white;
	int num_given_black;
	int depth_to_search;	// maximum search depth
	int x;
	int y;


    
	cin >> size;
//    size=9;
    
	cin >> depth_to_search;
//    depth_to_search=5;
    
//	boardOrigin = new int*[size];
//    
//	for( int i = 0; i < size; i++){
//		boardOrigin[i] = new int[size];
//	}
	for( int i = 0; i < size; i++){
		for( int j = 0; j < size; j++){
			boardOrigin[i][j] = 0;
		}
	}
    
	
	cin >> num_given_white;
//    num_given_white=4;
//    boardOrigin[5][5] = 1;
//    boardOrigin[4][5] = 1;
//    boardOrigin[6][3] = 1;
//    boardOrigin[6][4] = 1;
    
	for(int i = 0; i < num_given_white; i++){
		cin >> x;
		cin >> y;
		boardOrigin[x][y] = 1;
	}
    
    
	cin >> num_given_black;
//    num_given_black=4;

//    boardOrigin[0][0] = -1;
//    boardOrigin[8][8] = -1;
//    boardOrigin[0][8] = -1;
//    boardOrigin[8][0] = -1;
    
	for( int i = 0; i < num_given_black; i++){
		cin >> x;
		cin >> y;
		boardOrigin[x][y] = -1;
	}
    
    
    
//    time_t t1, t2;
//     time(&t1);
//     printf("Start: \t %s", ctime(&t1));
    
    
    

//    int rc;
//    void *pthread_status;
    int fuck,you,yeah,yo;
    int NUM_THREADS=depth_to_search;
    
    


    PV passValue[50];
    for(fuck=0;fuck<NUM_THREADS;fuck++){

        (passValue[fuck]).inputI=fuck+1;
//        int **newPrivateBoard;
        
//        newPrivateBoard = new int*[size];
//        
////        cout<<newPrivateBoard<<endl;
//        for(yo = 0; yo < size; yo++){
//            newPrivateBoard[yo] = new int[size];
//        }
//        passValue[fuck].inputBoard=newPrivateBoard;
        
//        cout<<newPrivateBoard<<endl;
//        cout<<passValue[fuck].inputBoard<<endl;
//        passValue[fuck]->inputBoard[2][2]=1;
//        cout<<passValue[fuck]->inputBoard[2][2];

        for( yo = 0; yo < size; yo++){
            for(yeah = 0; yeah < size; yeah++){

                inputBoard[fuck][yeah][yo] = boardOrigin[yeah][yo];
//                cout<<inputBoard[fuck][yeah][yo];
            }
//            cout<<endl;
        }
        
    }
    
    
    
    
    
    pthread_attr_t attr;
    pthread_t thread[50];
    

    
    
    long iStall=0;

    
    
    
    
    dieCounter=0;
    killAllThread=false;
    whoFinally=-99;
    depthFinally=-99;
    
    for(int i=0;i<NUM_THREADS;i++){
        nega_max_parallel(&passValue[i]);
    }
    
//    nega_max_parallel(&passValue[4]);

    

//    for(fuck=0;fuck<NUM_THREADS;fuck++){
//        pthread_attr_init(&attr);
//        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//        
////        printf("%d created\n",fuck);
//        pthread_create(&thread[fuck], &attr, nega_max_parallel, &passValue[fuck]);
//    }
//
//    
//    pthread_attr_destroy(&attr);
//    for(you=0;you<NUM_THREADS;you++){
////        printf("%d joined\n",you);
//        pthread_join(thread[you], NULL);
//
//    }
//
//    
//    
////    for(int i=0;i<NUM_THREADS;i++){
////        iStall=i+1;
////        nega_max_parallel((void *)iStall);
////    }
//
//
//    pthread_attr_destroy(&attr);
//    for(int i=0;i<NUM_THREADS;i++){
////        printf("%d joined\n",i);
//        pthread_join(thread[i], NULL);
//    }


    

    if(dieCounter==NUM_THREADS){//all dead
        cout << "draw";
    }else{
        if( whoFinally == -1 ) {
			cout << depthFinally << " " << "black";
		}
		else if( whoFinally == 1) {
			cout << depthFinally << " " << "white";
		}
    }
    
//    cout<<"fuckyou\n";
    
//    pthread_exit(NULL);
    
    
    
    
    
    
//     time(&t2);
//     printResults(&t1, &t2);
//	
	return 0;
}
