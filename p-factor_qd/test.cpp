#include <iostream>
#include <stack>
using namespace std;
int main() {
    stack<int> a;
    a.push(2);
    a.push(2);

    for(int i = 0; i < a.size(); i++) {
        cout << a.top() << endl;
        a.pop();
    }
    return 0;
}