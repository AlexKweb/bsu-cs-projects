#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

#include "logic.h"

int* array_data;
int array_size;
int marker_count;

pthread_mutex_t array_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t random_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t state_condition = PTHREAD_COND_INITIALIZER;

bool started = false;
int stopped_count = 0;
int active_count = 0;

// 0 - wait, 1 - continue, 2 - terminate
std::vector<int> commands;
std::vector<int> active;

void print_array()
{
    pthread_mutex_lock(&array_mutex);
    for (int i = 0; i < array_size; ++i)
        std::cout << array_data[i] << ' ';
    std::cout << '\n';
    pthread_mutex_unlock(&array_mutex);
}

void* marker(void* parameter)
{
    int id = *static_cast<int*>(parameter);

    pthread_mutex_lock(&state_mutex);
    while (!started)
        pthread_cond_wait(&state_condition, &state_mutex);
    pthread_mutex_unlock(&state_mutex);

    pthread_mutex_lock(&random_mutex);
    std::srand(id);
    pthread_mutex_unlock(&random_mutex);

    int marked_count = 0;

    while (true)
    {
        pthread_mutex_lock(&random_mutex);
        int index = std::rand() % array_size;
        pthread_mutex_unlock(&random_mutex);

        bool cannot_mark = false;

        pthread_mutex_lock(&array_mutex);

        if (is_free(array_data, index))
        {
            usleep(5 * 1000);
            array_data[index] = id;
            ++marked_count;
            usleep(5 * 1000);
        }
        else
        {
            cannot_mark = true;
        }

        pthread_mutex_unlock(&array_mutex);

        if (!cannot_mark)
            continue;

        pthread_mutex_lock(&output_mutex);
        std::cout << "Marker " << id
                  << ": marked = " << marked_count
                  << ", cannot mark index = " << index << '\n';
        pthread_mutex_unlock(&output_mutex);

        pthread_mutex_lock(&state_mutex);
        ++stopped_count;
        pthread_cond_broadcast(&state_condition);

        while (commands[id - 1] == 0)
            pthread_cond_wait(&state_condition, &state_mutex);

        int command = commands[id - 1];
        commands[id - 1] = 0;
        pthread_mutex_unlock(&state_mutex);

        if (command == 2)
        {
            pthread_mutex_lock(&array_mutex);
            clear_marks(array_data, array_size, id);
            pthread_mutex_unlock(&array_mutex);
            return nullptr;
        }
    }
}

int main()
{
    std::cout << "Array size: ";
    std::cin >> array_size;

    if (array_size <= 0)
    {
        std::cout << "Array size must be positive.\n";
        return 1;
    }

    array_data = new int[array_size]{};

    std::cout << "Marker count: ";
    std::cin >> marker_count;

    if (marker_count <= 0)
    {
        std::cout << "Marker count must be positive.\n";
        delete[] array_data;
        return 1;
    }

    active_count = marker_count;
    commands.assign(marker_count, 0);
    active.assign(marker_count, 1);

    std::vector<pthread_t> threads(marker_count);
    std::vector<int> ids(marker_count);

    for (int i = 0; i < marker_count; ++i)
    {
        ids[i] = i + 1;
        pthread_create(&threads[i], nullptr, marker, &ids[i]);
    }

    pthread_mutex_lock(&state_mutex);
    started = true;
    pthread_cond_broadcast(&state_condition);
    pthread_mutex_unlock(&state_mutex);

    while (active_count > 0)
    {
        pthread_mutex_lock(&state_mutex);
        while (stopped_count != active_count)
            pthread_cond_wait(&state_condition, &state_mutex);
        pthread_mutex_unlock(&state_mutex);

        std::cout << "\nArray:\n";
        print_array();

        int id;
        while (true)
        {
            std::cout << "Marker to terminate: ";
            std::cin >> id;

            if (id >= 1 && id <= marker_count && active[id - 1])
                break;

            std::cout << "This marker is not active.\n";
        }

        pthread_mutex_lock(&state_mutex);
        commands[id - 1] = 2;
        pthread_cond_broadcast(&state_condition);
        pthread_mutex_unlock(&state_mutex);

        pthread_join(threads[id - 1], nullptr);

        active[id - 1] = 0;
        --active_count;

        std::cout << "Array after marker " << id << " finished:\n";
        print_array();

        if (active_count == 0)
            break;

        pthread_mutex_lock(&state_mutex);
        stopped_count = 0;

        for (int i = 0; i < marker_count; ++i)
        {
            if (active[i])
                commands[i] = 1;
        }

        pthread_cond_broadcast(&state_condition);
        pthread_mutex_unlock(&state_mutex);
    }

    delete[] array_data;

    pthread_mutex_destroy(&array_mutex);
    pthread_mutex_destroy(&state_mutex);
    pthread_mutex_destroy(&output_mutex);
    pthread_mutex_destroy(&random_mutex);
    pthread_cond_destroy(&state_condition);

    return 0;
}
