#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

int findroot(int index);
int *disjoin_array;
int index_1,index_2;

int main(int argc,char* argv[])
{
    int number,count;
    char line[1000000];
    char *pch;

    cin >> number;
    disjoin_array = new int[number];

    for(int i=0;i<number;i++)
        disjoin_array[i] = i;

    while(gets(line)){
        pch = strtok(line,"->");
        if(pch != NULL)
        {
            index_1 = findroot(atoi(pch));
            pch = strtok (NULL,"->,");
        }
        while(pch != NULL)
        {
            index_2 = findroot(atoi(pch));
            index_1 > index_2 ? disjoin_array[ index_1 ] = index_2 : disjoin_array[ index_2 ] = index_1;
            pch = strtok (NULL, ",");
        }
    }

    count=0;
    for(int i=0;i<number;i++){
        //disjoin_array[i] = findroot(i);
        if(disjoin_array[i]==i) count++;
    }

    cout << count << endl;

    return 0;
}

int findroot(int _index)
{
    if(disjoin_array[_index] != _index)
        disjoin_array[_index] = findroot(disjoin_array[_index]);
    return disjoin_array[_index];
}
