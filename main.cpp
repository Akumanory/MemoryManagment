#include <iostream>
#include <string>

#include "fsa_old.cpp"
#include "test_classes.h"


int main() {
    // MemoryManager a;
    FSA a(16);
    a.init();

    std::cout << "sizeof(Test_Class): " << sizeof(Test_Class) <<std::endl;

    Test_Class* first =  (Test_Class*)a.alloc(sizeof(Test_Class));
    Test_Class* second = (Test_Class*)a.alloc(sizeof(Test_Class));
    Test_Class* third =  (Test_Class*)a.alloc(sizeof(Test_Class));

    a.show_mapped();
    
    first = new (first) Test_Class(101,1001, "first");
    second = new (second) Test_Class(202,2002, "second");
    // *(first) = 14;
    // *(second) = 12;

    Test_Class* fourth = (Test_Class*)a.alloc(sizeof(Test_Class));
    Test_Class* fith =   (Test_Class*)a.alloc(sizeof(Test_Class));
    Test_Class* sixh =   (Test_Class*)a.alloc(sizeof(Test_Class));
    a.show_mapped();

    Test_Class* seventh = (Test_Class*)a.alloc(sizeof(Test_Class));
    
    a.show_mapped();

    std::cout << "First: " << first->test_int << std::endl;
    std::cout << "Second: " << second->test_int << std::endl;
    // std::cout << "Third: " << *(third) << std::endl;

    // // a.free(seventh);

    // a.free(first);

    // int* temp = (int*)a.alloc(sizeof(int));
    // *(temp) = 50505;
    // a.show_mapped();
    // a.free(seventh);
    // a.show_mapped();
    // int* temp_2 = (int*)a.alloc(sizeof(int));
    // *(temp_2) = 10101010;
    // a.show_mapped();
    // // a.free(temp_2);
    // // a.show_mapped();

    // // a.free(second);
    // // int* first_second = (int*)a.alloc(sizeof(int));
    // // *(first_second) = 15;
    // // *(third) = 26;

    
    // // std::cout << "First: " << *(first) << std::endl;
    // // std::cout << "first_second: " << *(first_second) << std::endl;
    // // std::cout << "Second: " << *(second) << std::endl;
    // // std::cout << "Third: " << *(third) << std::endl;

    

    a.destroy();
}