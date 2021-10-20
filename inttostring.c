#include <stdio.h> //test string
#include <assert.h>
#include <string.h>

#define ZERO 48

void inttostring(int n, char string_[]);
void test(void);

int main(void)
{
    test();
    int n;
    printf("Integer: ");
    scanf("%d", &n);

    char str[100];

    inttostring(n, str);

    printf("String: %s\n", str);
    return 0;
}

void inttostring(int n, char string_[])
{
    int num[100];
    char string[100];
    num[0] = n % 10;

    string[0] = (char)(num[0] + ZERO);
    int count = 1;
    for (int i = 1; n > 0; i++)
    {
        n = (n - num[i - 1]) / 10;
        num[i] = n % 10;

        char character = (char)(num[i] + ZERO);

        string[i] = character;
        count++; //to reverse printing
    }

    if (string[count - 1] == '0')
    { //delete last element which is 0
        string[count - 1] = '\0';
    }
    count--;

    for (int j = 0; j < count; j++)
    {
        string_[j] = string[count - 1 - j];
    }
}

void test(void)
{
    int n = 123546;
    char str_test1[10];
    inttostring(n, str_test1);
    assert(strcmp("123546", str_test1) == 0);
    assert(strcmp("123456777", str_test1) > 0 || strcmp("123456777", str_test1) < 0);
    assert(strcmp("123456777", str_test1) != 0);

    int m = 999990;
    char str_test2[10];
    inttostring(m, str_test2);
    assert(strcmp("999990", str_test2) == 0);
    assert(strcmp("1", str_test2) > 0 || strcmp("123456777", str_test2) < 0);
    assert(strcmp("1", str_test2) != 0);

    printf("test done\n");
}