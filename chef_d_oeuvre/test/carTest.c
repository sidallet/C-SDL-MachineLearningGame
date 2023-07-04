#include <assert.h>
#include <stdio.h>

void carTestCase(void) {
    assert(1 == 1);
}

#ifdef UNIT_TEST

int main(){
    carTestCase();
    
    printf("All tests passed !\n");
    return 0;
}

#endif

