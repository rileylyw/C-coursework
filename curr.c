#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define PRECISION 0.0001

bool is_equal(float a, float b);
float get_curr(char cur_type);
void test(void);

int main(void)
{
    test();
    char cur_type;
    printf("Choose currency type:\np: GBP\nd: USD\ne: EUR\n");
    cur_type = getchar();
    float rate = get_curr(cur_type);
    printf("The exchange rate for %c is %f\n", cur_type, rate);
    return 0;
}

float get_curr(char cur_type){
    switch (cur_type)
    {
    case 'p':
        return 1.0;
    case 'd':
        return 1.3;
    case 'e':
        return 1.11;
    default:
        return -1;
    }
}

bool is_equal(float a, float b){
    if(a>=b-PRECISION && a<=b+PRECISION){
        return true;
    }
    else{
        return false;
    }
}

void test(void){
    assert(is_equal(get_curr('p'), 1.0)==true);
    assert(is_equal(get_curr('d'), 1.3)==true);
    assert(is_equal(get_curr('e'), 1.11)==true);
    assert(is_equal(get_curr('d'), 1.11)==false);
    assert(is_equal(get_curr('?'), -1)==true);
    assert(is_equal(get_curr(123), -1)==true);

    assert(is_equal(1.00, 1.00)==true);
    assert(is_equal(1.00, 1.000)==true);
    assert(is_equal(1, 1.00)==true);
    assert(is_equal(10, 1.00)==false);

    // overfloat testing
	assert(is_equal(3.402823e+38, 3.402823e+38) == true);
	assert(is_equal(1.175494e-38, 3.402823e+38) == false);
}