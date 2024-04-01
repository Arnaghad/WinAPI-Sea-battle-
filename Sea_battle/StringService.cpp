#include "StringService.h"

pair<char, int> split_string(string s) {
    // Loop through each character of the string
    string y_str = "";
    string x_str = "";
    for (char c : s) {
        // If the character is a letter, append it to x
        if (isalpha(c)) {
            x_str += c;
        }
        // If the character is a digit, append it to y
        else if (isdigit(c)) {
            y_str += c;
        }
    }
    char x = x_str[0];
    int y = stoi(y_str);
    return make_pair(x, y);
}