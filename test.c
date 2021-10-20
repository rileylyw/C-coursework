#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define BIGSTR 1000
#define GRID 100

struct crossword
{
    char arr[GRID][GRID];
    int sz;
};
typedef struct crossword crossword;

void test(void);

int main(int sz, char *ip, crossword *cw)
{
    test();
    if (ip == NULL || cw == NULL)
    {
        return "false";
    }
    if ((int)strlen(ip) != sz * sz || sz < 0)
    {
        return "false";
    }
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
        {
            cw->arr[i][j] = ip[i * sz + j];
            printf("%c", cw->arr[i][j]);
        }
    }
    cw->sz = sz;
}

void test(void)
{
    assert(str2crossword(5, "....X.XX.X.X......X.XX...", &c));
    getcluestring(&c, str);
    assert(strcmp("A-1-3-5-6|D-1-2-3-4", str) == 0);
    assert(getchecked(c) == 53);
    printf("test done");
}