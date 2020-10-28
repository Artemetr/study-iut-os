#include <iostream>

using namespace std;

void init_user_matrix(int** user_matrix, int& x, int& y) {
    int value;

    cout << "Input columns count: ";
    cin >> x;

    cout << "Input rows count: ";
    cin >> y;

    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            cin >> value;
            user_matrix[i][j] = value;
        }
    }
}

int main() {
    int** user_matrix;
    int x, y;

    init_user_matrix(user_matrix, x, y);

    return EXIT_SUCCESS;
}