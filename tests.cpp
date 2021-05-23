#include <string>
#include <gtest/gtest.h>

#include "fsa_old.cpp"
#include "test_classes.h"


TEST(TestWithClasses, CompareFields)
{
    FSA a(128);
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
    // a.show_mapped();

    Test_Class* seventh = (Test_Class*)a.alloc(sizeof(Test_Class));
    
    // a.show_mapped();

    std::cout << "First: " << first->test_int << std::endl;
    std::cout << "Second: " << second->test_int << std::endl;

    ASSERT_EQ(first->test_int, 101);
    ASSERT_EQ(first->test_int2, 1001);
    ASSERT_EQ(first->test_str, "first");
    ASSERT_EQ(second->test_int, 202);
    ASSERT_EQ(second->test_int2, 2002);
    ASSERT_EQ(second->test_str, "second");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}