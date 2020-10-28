#include <iostream>

using namespace std;

int main() {
    int month;
    int month_to_day[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    cout << "Input month number: ";
    try {
        cin >> month;

        month--;
        if (month > 11 or month < 0) {
            throw 1;
        }
    } catch (...) {
        cout << "Error: invalid month number" << endl;

        system("pause");
        return EXIT_FAILURE;
    }

    cout << "Days count of inputted month: " << month_to_day[month] << endl;
    
    system("pause");
    return EXIT_SUCCESS;
}
