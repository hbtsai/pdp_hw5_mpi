#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int n, m, k;
char equation[20][50];

typedef struct dfs_arg {
    pthread_t tid;
    int visited[20];
    int varible[20];
    int ans;

} dfs_arg;

void check(dfs_arg *args)
{
    int len;
    long long int left;
    long long int right;
    long long int tmp;
    long long int mul_num;
    long long int add_num;
    char c;

    for(int i = 0; i < k; i++) {
        len = strlen(equation[i]);
        tmp = 0;
        mul_num = 1;
        add_num = 0;
        for(int j = 0; j < len; j++) {
            c = equation[i][j];
            if(c >= 'A' && c <= 'Z') {
                tmp *= n;
                tmp += args->varible[c - 'A'];
            } else if(c == '+') {
                mul_num *= tmp;
                add_num += mul_num;
                tmp = 0;
                mul_num = 1;
            } else if(c == '*') {
                mul_num *= tmp;
                tmp = 0;
            } else if(c == '=') {
                mul_num *= tmp;
                add_num += mul_num;
                left = add_num;
                tmp = 0;
                mul_num = 1;
                add_num = 0;
            }
        }
        mul_num *= tmp;
        add_num += mul_num;
        right = add_num;
        if(right != left) {
            return;
        }
    }
    args->ans++;
    return;
}

void DFS(int step, dfs_arg *args)
{
    if(step == m) {
        /*for(int i = 0; i < m; i++) {
            printf("%d ", args->varible[i]);
        }
        printf("\n");*/
        check(args);
        return;
    }
    for(int i = 0; i < n; i++) {
        if(args->visited[i] == 0){
              args->visited[i] = 1;
              args->varible[step] = i;
              DFS(step + 1, args);
              args->visited[i] = 0;
          }
    }
    return;
}

void* dfs_thread(void* param)
{
    dfs_arg *arg = (dfs_arg*) param;
    DFS(1, arg);

}

void dfs_main(dfs_arg *args, int first)
{
    for(int i = 0; i < n; i++) {
        args->visited[i] = 0;
        args->varible[i] = 0;
    }
    args->visited[first] = 1;
    args->varible[0] = first;
    args->ans = 0;

    pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&args->tid, &attr, dfs_thread, args);
}

int main()
{
    int ans = 0;
    scanf("%d%d%d", &n, &m, &k);
    for(int i = 0; i < k; i++) {
        scanf("%s", equation[i]);
    }

    dfs_arg *args = 0;
    if(args == 0) {
        args = (dfs_arg*)malloc(sizeof(dfs_arg)*n);
    }

    //init();
    for(int i = 0; i < n; i++) {
        dfs_main(args+i, i);
    }

    for(int i = 0; i < n; i++) {
		pthread_join(args[i].tid, (void**) 0);
		ans += args[i].ans;
	}

    //DFS(1);
    printf("%d", ans);

    return 0;
}
