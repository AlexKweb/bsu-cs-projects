#include <cassert>
#include <iostream>

#include "logic.h"

void test_free_element()
{
    int array[] = {0, 2, 0};
    assert(is_free(array, 0));
}

void test_busy_element()
{
    int array[] = {0, 2, 0};
    assert(!is_free(array, 1));
}

void test_clear_marks()
{
    int array[] = {1, 2, 1, 3, 1};

    clear_marks(array, 5, 1);

    assert(array[0] == 0);
    assert(array[1] == 2);
    assert(array[2] == 0);
    assert(array[3] == 3);
    assert(array[4] == 0);
}

int main()
{
    test_free_element();
    test_busy_element();
    test_clear_marks();

    std::cout << "3 tests passed\n";
    return 0;
}
