#include <iostream>
using namespace std;

int main() {
char lletra;
    int countA = 0;

    while (cin >> lletra && lletra != '.') {
        if (lletra == 'a') {
            countA++;
        }
    }

    cout << countA << endl;

    return 0;
}
