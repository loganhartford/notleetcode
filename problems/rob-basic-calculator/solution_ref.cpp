#include <string>
#include <stack>

int calculate(std::string s) {
    std::stack<int> stk;
    int num = 0;
    char op = '+';
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        }
        if ((!isdigit(c) && c != ' ') || i == (int)s.size()-1) {
            if (op == '+') stk.push(num);
            else if (op == '-') stk.push(-num);
            else if (op == '*') { int top=stk.top(); stk.pop(); stk.push(top*num); }
            else if (op == '/') { int top=stk.top(); stk.pop(); stk.push(top/num); }
            op = c;
            num = 0;
        }
    }
    int result = 0;
    while (!stk.empty()) { result += stk.top(); stk.pop(); }
    return result;
}
