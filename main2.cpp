#include <iostream>
// #include "memory_manager.h"
#include "coalesce_old.cpp"

int main()
{
    Coalesce a;
    a.init();

    int* first = (int*)a.alloc(sizeof(int));
    int* second = (int*)a.alloc(sizeof(int));
    int* third = (int*)a.alloc(sizeof(int));
    int* fourth = (int*)a.alloc(sizeof(int));
    int* fith = (int*)a.alloc(sizeof(int));

    *(first) = 101;
    *(second) = 202;
    *(third) = 303;
    *(fourth) = 404;
    *(fith) = 505;

    a.free(second);
    a.free(fourth);
    
    // a.show_data();

    a.free(third);

    // a.show_data();

    int* first_2 = (int*)a.alloc(sizeof(int));

    *(first_2) = 2101;

    a.show_data();

    a.destroy();
}