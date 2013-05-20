#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"
int g_n,g_m,g_k;
char **equation;

size_t my_strlen(const char *s) {
    size_t len = 0;
    for(;;) {
        unsigned x = *(unsigned*)s;
        if((x & 0xFF) == 0) return len;
        if((x & 0xFF00) == 0) return len + 1;
        if((x & 0xFF0000) == 0) return len + 2;
        if((x & 0xFF000000) == 0) return len + 3;
        s += 4, len += 4;
    }
}

void print_equation(){
	int k_i;
	int sl,sl_i;
  //xx
  int varible[26] = {0};
  //xx
	for( k_i = 0 ; k_i < g_k ; ++k_i ){
		sl = my_strlen(equation[k_i]);
		for( sl_i = 0 ; sl_i < sl ; ++sl_i ){
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

int check_equation(int *var_lc){
	int k_i, n = g_n, k = g_k;
	int sl,sl_i;
	int left,right,add_temp,mul_temp;
  register int temp;
	for( k_i = 0 ; k_i < k ; ++k_i ){
		temp = 0;
		mul_temp = 1;
		add_temp = 0;
		sl = my_strlen(equation[k_i]);
		for( sl_i = 0 ; sl_i < sl ; ++sl_i ){
      register char equtmp = equation[k_i][sl_i];
			if( equtmp >= 'A' && equtmp <= 'Z' ){
				temp *= n;
				temp += var_lc[equtmp - 'A'];
			}
			if( equtmp == '*' ){
				mul_temp *= temp;
				temp = 0;
			}
			if( equtmp == '+' ){
				mul_temp *= temp;
				add_temp += mul_temp;
				temp = 0;
				mul_temp = 1;
			}
			if( equtmp == '=' ){
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
		if( right != left){
			return 0;
		}
	}

	return 1;
}

void generate_varibles(long long a, int *var_lc){
	long long temp_a = a;
	register int m_i;
  int m_j,n = g_n, m = g_m;
	for( m_i = 0 ; m_i < m ; ++m_i ){
		var_lc[m-1-m_i] = temp_a%(n-m+m_i+1);
		temp_a = temp_a/(n-m+m_i+1);
	}
	for( m_i = m-2 ; m_i >= 0 ; --m_i ){
		for( m_j = m_i+1 ; m_j < m ; ++m_j ){
			if( var_lc[m_i] <= var_lc[m_j] ){
				++var_lc[m_j];
			}
		}
	}
	return;
}

void generate_next(long long a, int *var_lc){
	register int n_i,m_i;
  int n = g_n, m = g_m;
	for( n_i = var_lc[m-1]+1 ; n_i < n ; ++n_i ){
		for( m_i = 0 ; m_i < m-1 ; ++m_i ){
			if( n_i == var_lc[m_i] ){
				break;
			}
		}
		if( m_i >= m-1 ){
			var_lc[m-1] = n_i;
			return;
		}
	}
	if( n_i >= n ){
		generate_varibles(a, var_lc);
	}
	return;
}

int main(){
	//read input
	{
		scanf("%d %d %d",&g_n,&g_m,&g_k);
		int k_i;
		equation = (char **)malloc(sizeof(long long)*g_k);
		for( k_i = 0 ; k_i < g_k; ++k_i ){
			equation[k_i] = (char *)malloc(sizeof(char)*256);
			scanf("%s",equation[k_i]);
		}
	}
  
  
	//compute all case:a
	static long long a = 1;
	{
		int m_i;
		for( m_i = g_n ; m_i > g_n-g_m ; --m_i ){
			a *= m_i;
		}
	}

	//computing
	unsigned int s = 0, _s = 0;
  long long a_j = -100, a_i;
  int varible[26] = {0};
  int nt = omp_get_num_procs();
  //omp_set_num_threads(nt);
#pragma omp parallel private(a_j,s) firstprivate(varible) reduction(+:_s)
  {
    s = 0;
    a_j = -100;
#pragma omp for schedule(guided, nt+2)
  for( a_i = 0; a_i < a ; ++a_i ){
		//generate varibles
		if( a_j == a_i-1 )
			generate_next(a_i, varible);
		else
			generate_varibles(a_i, varible);

		a_j = a_i;

		//check & record
		if( check_equation(varible) )
			++s;
  }
  _s = _s + s;
  }
	printf("%d",_s);

	return 0;
}
