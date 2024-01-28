#include <iostream>
#include <iomanip>
using namespace std;

int main() {
double suma = 0.0;
double x;
int count = 0;

    while (cin >> x && x > 0) {
        suma += x;
        count++;
    }

if (count > 0) {
        double average = suma / count;
cout << fixed <<setprecision(2) << average <<endl;



    return 0;
}
