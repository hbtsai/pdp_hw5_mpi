#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n,m,k;
char **equation;
int varible[26];
long long *max_alphabet;
long long a;
char mapping[26];

void print_equation(){
	int k_i;
	int sl,sl_i;
	for( k_i = 0 ; k_i < k*2 ; k_i++ ){
		sl = strlen(equation[k_i]);
		for( sl_i = 0 ; sl_i < sl ; sl_i++ ){
			if( equation[k_i][sl_i] >= 'A' && equation[k_i][sl_i] <= 'Z' ){
				printf("(%d)",varible[equation[k_i][sl_i]-'A']);
			}
			else{
				printf("%c",equation[k_i][sl_i]);
			}
		}
		printf("\n");
	}
	return;
}

int check_equation(){
	int k_i;
	int sl,sl_i;
	int left,right,add_temp,mul_temp,temp;
	for( k_i = k ; k_i < k*2 ; k_i++ ){
		temp = 0;
		mul_temp = 1;
		add_temp = 0;
		sl = strlen(equation[k_i]);
		for( sl_i = 0 ; sl_i < sl ; sl_i++ ){
			if( equation[k_i][sl_i] >= 'A' && equation[k_i][sl_i] <= 'Z' ){
				//temp *= n;
				temp = varible[equation[k_i][sl_i] - 'A'];
			}
			if( equation[k_i][sl_i] == '*' ){
				mul_temp *= temp;
				//temp = 0;
			}
			if( equation[k_i][sl_i] == '+' ){
				mul_temp *= temp;
				add_temp += mul_temp;
				//temp = 0;
				mul_temp = 1;
			}
			if( equation[k_i][sl_i] == '=' ){
				mul_temp *= temp;
				add_temp += mul_temp;
				left = add_temp;
				//temp = 0;
				mul_temp = 1;
				add_temp = 0;
			}
		}
		mul_temp *= temp;
		add_temp += mul_temp;
		right = add_temp;
		if( right%n != left%n ){
			return max_alphabet[(k_i-k)*2+1];
		}
	}
	for( k_i = 0 ; k_i < k ; k_i++ ){
		temp = 0;
		mul_temp = 1;
		add_temp = 0;
		sl = strlen(equation[k_i]);
		for( sl_i = 0 ; sl_i < sl ; sl_i++ ){
			if( equation[k_i][sl_i] >= 'A' && equation[k_i][sl_i] <= 'Z' ){
				temp *= n;
				temp += varible[equation[k_i][sl_i] - 'A'];
			}
			if( equation[k_i][sl_i] == '*' ){
				mul_temp *= temp;
				temp = 0;
			}
			if( equation[k_i][sl_i] == '+' ){
				mul_temp *= temp;
				add_temp += mul_temp;
				temp = 0;
				mul_temp = 1;
			}
			if( equation[k_i][sl_i] == '=' ){
				mul_temp *= temp;
				add_temp += mul_temp;
				left = add_temp;
				temp = 0;
				mul_temp = 1;
				add_temp = 0;
			}
		}
		mul_temp *= temp;
		add_temp += mul_temp;
		right = add_temp;
		if( right != left ){
			return 1;
		}
	}

	return 0;
}

void generate_varibles(long long a){
	long long temp_a;
	temp_a = a;
	int m_i,m_j;
	for( m_i = 0 ; m_i < m ; m_i++ ){
		varible[m-1-m_i] = temp_a%(n-m+m_i+1);
		temp_a = temp_a/(n-m+m_i+1);
	}
	for( m_i = m-2 ; m_i >= 0 ; m_i-- ){
		for( m_j = m_i+1 ; m_j < m ; m_j++ ){
			if( varible[m_i] <= varible[m_j] ){
				varible[m_j]++;
			}
		}
	}
	return;
}

void generate_next(long long a){
	int n_i,m_i;
	for( n_i = varible[m-1]+1 ; n_i < n ; n_i++ ){
		for( m_i = 0 ; m_i < m-1 ; m_i++ ){
			if( n_i == varible[m_i] ){
				break;
			}
		}
		if( m_i >= m-1 ){
			varible[m-1] = n_i;
			return;
		}
	}
	if( n_i >= n ){
		generate_varibles(a);
	}
	return;
}

int main()
{
	int rank=0;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank==0)
	{

	//read input
	{
		scanf("%d %d %d",&n,&m,&k);
		int k_i;
		equation = (char **)malloc(sizeof(long long)*k*2);
		for( k_i = 0 ; k_i < k ; k_i++ ){
			equation[k_i] = (char *)malloc(sizeof(char)*256);
			scanf("%s",equation[k_i]);
		}
		for( k_i = 0 ; k_i < k ; k_i++ ){
			equation[k_i+k] = (char *)malloc(sizeof(char)*256);
		}
	}

	//mapping
	char to_change = 'A';
	int index = 0;
	for( int c_i = 0 ; c_i < 26 ; c_i++ ){
		mapping[c_i] = c_i+'A';
	}
	for( int k_i = 0 ; k_i < k ; k_i++ ){
		int sl = strlen(equation[k_i]);
		int sl2 = 0;
		char temp = equation[k_i][0];
		char temp2;
		for( int sl_i  = 1 ; sl_i < sl+1 ; sl_i++ ){
			temp2 = equation[k_i][sl_i];
			if( (temp >= 'A' && temp <= 'Z') && (temp2 == '=' || temp2 == '+' || temp2 == '*' || temp2 == '\0') ){
				if( mapping[temp-'A'] >= to_change ){
					for( int c_i = 0 ; c_i < 26 ; c_i++ ){
						if( mapping[c_i] == to_change ){
							index = c_i;
						}
					}
					mapping[temp-'A'] = mapping[index];
					mapping[index] = temp;
					to_change++;
				}
			}
			temp = temp2;
		}
	}
	//for( int c_i = 0 ; c_i < 26 ; c_i++ ){
	//	printf("%c ",mapping[c_i]);
	//}
	//printf("\n");
	for( int k_i = 0 ; k_i < k ; k_i++ ){
		int sl = strlen(equation[k_i])+1;
		for( int sl_i = 0 ; sl_i < sl ; sl_i++ ){
			if( equation[k_i][sl_i] >= 'A' && equation[k_i][sl_i] <= 'Z' ){
				equation[k_i][sl_i] = mapping[equation[k_i][sl_i]-'A'];
			}
		}
	}
	
	//compute all case:a
	//long long a = 1;
	a = 1;
	{
		int m_i;
		for( m_i = n ; m_i > n-m ; m_i-- ){
			a *= m_i;
		}
	}

	//generate simple equation
	max_alphabet = (long long *)malloc(sizeof(long long)*k*2);;
	for( int k_i = 0 ; k_i < k ; k_i++ ){
		int sl = strlen(equation[k_i]);
		int sl2 = 0;
		char temp = equation[k_i][0];
		char temp2;
		for( int sl_i  = 1 ; sl_i < sl+1 ; sl_i++ ){
			temp2 = equation[k_i][sl_i];
			if( (temp >= 'A' && temp <= 'Z') && (temp2 == '=' || temp2 == '+' || temp2 == '*' || temp2 == '\0') ){
				equation[k_i+k][sl2] = temp;
				sl2++;
			}
			if( (temp2 >= 'A' && temp2 <= 'Z') && (temp == '=' || temp == '+' || temp == '*' || temp == '\0') ){
				equation[k_i+k][sl2] = temp;
				sl2++;
			}
			temp = temp2;
		}
	}
	for( int k_i = k ; k_i < k*2 ; k_i++ ){
		char max = equation[k_i][0];
		int sl = strlen(equation[k_i])+1;
		for( int sl_i = 1 ; sl_i < sl ; sl_i++ ){
			if( equation[k_i][sl_i] > max && equation[k_i][sl_i] <= 'Z' ){
				max = equation[k_i][sl_i];
			}
		}
		max_alphabet[(k_i-k)*2] = max -'A' + 1;
		long long small_a = 1;
		for( int c_i = 0 ; c_i < max_alphabet[(k_i-k)*2] ; c_i++ ){
			small_a *= (n-c_i);
		}
		max_alphabet[(k_i-k)*2+1] = a/small_a;
		//printf("%c %lld\n",max,max_alphabet[(k_i-k)*2+1]);
	}

	//computing
	int s = 0;
	long long a_i,a_j = -100;
	long long small_a = 0;
	for( a_i = 0 ; a_i < a ; a_i++ ){
		//generate varibles
		if( a_j == a_i-1 ){
			generate_next(a_i);
		}
		else{
			generate_varibles(a_i);
		}
		a_j = a_i;

		//check & record
		//print_equation();
		small_a = check_equation();
		if( check_equation() == 0 ){//1:if right answer,0:if wrong answer
			s++;
		}
		else if( small_a >= 2 ){
			a_i = (a_i/small_a+1)*small_a-1;
		}

	}

	printf("%d",s);
	}
	MPI_Finalize();

	return 0;
}
