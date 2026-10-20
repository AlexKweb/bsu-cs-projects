#pragma once

inline bool is_free(const int* array, int index)
{
    return array[index] == 0;
}

inline void clear_marks(int* array, int size, int marker_id)
{
    for (int i = 0; i < size; ++i)
    {
        if (array[i] == marker_id)
            array[i] = 0;
    }
}
