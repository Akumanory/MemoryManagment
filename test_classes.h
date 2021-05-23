#include <string>

class Test_Class {
public:

    // void* поля

    Test_Class() {}
    Test_Class(int _test_int,
             int _test_int2, 
             std::string _test_str )
            : test_int(_test_int)
            , test_int2(_test_int2)
            , test_str(_test_str)
        {}

    ~Test_Class() noexcept {}

    int test_int;
    int test_int2;
    std::string test_str;
};