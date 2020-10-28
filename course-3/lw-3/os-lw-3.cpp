#include <iostream>
#include <thread>

using namespace std;

void initUserMatrix(int**& userMatrix, int& size) {
    int value;

    cout << "Input matrix size: ";
    cin >> size;
    if (!size) {
        throw "Invalid matrix size";
    }

    userMatrix = new int*[size];
    for (int i = 0; i < size; i++) {
        userMatrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            cin >> value;
            if (!value) {
                throw "Invalid matrix value";
            }
            userMatrix[i][j] = value;
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

int main() {
    int** userMatrix;
    int result;
    int size;

    try {
        initUserMatrix(userMatrix, size);
    } catch (const char* e) {
        cout << "Error: " << e << endl;
        return EXIT_FAILURE;
    }

    thread th(calculateBorderElementsSummOfMatrix, userMatrix, size, ref(result));
    if (!th.joinable()) {
        cout << "Error with thread starting." << endl;
        return EXIT_FAILURE;
    }
    th.join();

    if (!result) {
        cout << "Error with invalide thread response." << endl;
        return EXIT_FAILURE;
    }

    cout << "Summ of matrix border elements: " << result << endl;

    return EXIT_SUCCESS;
}
