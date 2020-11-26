#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

void initRandomMatrix(int**& randomMatrix, int& size) {
    size = 1024;
    randomMatrix = new int*[size];
    for (int i = 0; i < size; i++) {
        randomMatrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            randomMatrix[i][j] = rand() % 2000;
        }
    }
}

void calculateBorderElementsSummOfMatrix(int** userMatrix, int size, int& result) {
    result = 0;
    for (int i = 0; i < size; i++) {
        if (i == 0 || i == size - 1) {
            for (int j = 0; j < size; j++) {
                result += userMatrix[i][j];
            }
        } else {
            result += userMatrix[i][0] + userMatrix[i][size - 1];
        }
    }
}

void runThread(int** matrix, int size, int priority, int needPolicy) {
    sched_param sch;
    int result;
    int policy;

    thread th(calculateBorderElementsSummOfMatrix, matrix, size, ref(result));
    pthread_getschedparam(th.native_handle(), &policy, &sch);
    sch.sched_priority = priority;
    policy = needPolicy;

    if (!th.joinable()) {
        cout << "Error with thread starting." << endl;
        exit(EXIT_FAILURE);
    }

    auto start = chrono::steady_clock::now();
    th.join();
    auto end = chrono::steady_clock::now();
    auto diff = end - start;

    cout << "," << chrono::duration <double, nano> (diff).count();

    if (!result) {
        cout << "Error with invalid thread response." << endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    int priorities [] = {1, -1, 2, -15, -2, 0, 15};
    int politics [] = {SCHED_FIFO, SCHED_RR};
    int** randomMatrix;
    int size;

    try {
        initRandomMatrix(randomMatrix, size);
    } catch (const char* e) {
        cout << "Error: " << e << endl;
        return EXIT_FAILURE;
    }

    for (int j : politics) {
        for (int i : priorities) {
            cout << j << "," << i;
            for (int t = 0; t < 16; t++) {
                runThread(randomMatrix, size, j, i);
            }
            cout << endl;
        }
    }

    return EXIT_SUCCESS;
}
