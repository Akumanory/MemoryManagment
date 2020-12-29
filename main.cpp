#include <iostream>
// #include "memory_manager.h"
#include "fsa_old.cpp"

int main() {
    // MemoryManager a;
    FSA a(16);
    a.init();
    int* first = (int*)a.alloc(sizeof(int));
    int* second = (int*)a.alloc(sizeof(int));
    int* third = (int*)a.alloc(sizeof(int));

    a.show_mapped();
    
    *(first) = 14;
    *(second) = 12;

    int* fourth = (int*)a.alloc(sizeof(int));
    int* fith = (int*)a.alloc(sizeof(int));
    int* sixh = (int*)a.alloc(sizeof(int));
    a.show_mapped();

    int* seventh = (int*)a.alloc(sizeof(int));
    
    a.show_mapped();

    // std::cout << "First: " << *(first) << std::endl;
    // std::cout << "Second: " << *(second) << std::endl;
    // std::cout << "Third: " << *(third) << std::endl;

    // a.free(seventh);

    a.free(first);

    int* temp = (int*)a.alloc(sizeof(int));
    *(temp) = 50505;
    a.show_mapped();
    a.free(seventh);
    a.show_mapped();
    int* temp_2 = (int*)a.alloc(sizeof(int));
    *(temp_2) = 10101010;
    a.show_mapped();
    // a.free(temp_2);
    // a.show_mapped();

    // a.free(second);
    // int* first_second = (int*)a.alloc(sizeof(int));
    // *(first_second) = 15;
    // *(third) = 26;

    
    // std::cout << "First: " << *(first) << std::endl;
    // std::cout << "first_second: " << *(first_second) << std::endl;
    // std::cout << "Second: " << *(second) << std::endl;
    // std::cout << "Third: " << *(third) << std::endl;

    

    a.destroy();
}