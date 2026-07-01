#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#pragma warning(disable: 4996)
using namespace std;

// char* func(char* s, int& num, int& numInsert) {//qw\0 w

//     char* word = new char[32];
//     char* wordInsert = new char[32];

//     int newNum = 0;
//     int counter = 0;
//     while (num > 0)
//     {
//         newNum = newNum*10 + num % 10;
//         counter++;
//         num/=10;
//     }
//     for (size_t i = 0; i < counter; i++)
//     {
//         word[i] = '0' + newNum % 10;
//         newNum /= 10;
//     }

//     newNum = 0;
//     counter = 0;
//     while (numInsert > 0)
//     {
//         newNum = newNum*10 + numInsert % 10;
//         counter++;
//         numInsert/=10;
//     }
//     for (size_t i = 0; i < counter; i++)
//     {
//         wordInsert[i] = '0' + newNum % 10;
//         newNum /= 10;
//     }
//     cout << word << " " << wordInsert << endl;
    
//     if (strstr(s, word) != nullptr) {
//         char* found_ptr = strstr(s, word);

//         size_t index = found_ptr - s;
//         int gap = strlen(wordInsert);
//         int lenS = strlen(s);
        
//         char tempStr[256]{};
//         for (size_t i = found_ptr - s + strlen(word), k = 0; i < lenS; i++, k++)
//         {   
//             tempStr[k] = s[i];
//         }
//         //cout << "1." << tempStr << "<=="<< endl;

//         for (size_t i = found_ptr - s, k=0; i < found_ptr - s + gap +1; i++, k++)
//         {   
//             s[i] = wordInsert[k];
//             s[i+1] = '\0';
//         }
//         //cout << "2." << s << "<=="<< endl;
//         for (size_t i = found_ptr - s + gap, k = 0; k < strlen(tempStr) ; i++, k++)
//         {   
//             s[i] = tempStr[k];
//         }
//         //cout << "3." << s << "<=="<< endl;
//     }
//     return s;
// }

char* func(char* s, int& num, int& numInsert) {
    char word[32];
    char wordInsert[32];

    sprintf(word, "%d", num);
    sprintf(wordInsert, "%d", numInsert);

    char* pos = strstr(s, word);
    if (!pos) return s;

    size_t wlen = strlen(word);
    size_t ilen = strlen(wordInsert);
    size_t tail = strlen(pos + wlen);

    memmove(pos + wlen + ilen, pos + wlen, tail + 1);
    memcpy(pos + wlen, wordInsert, ilen);

    return s;
}

int main() {
    char s[256];
    int word, wordInsert;

    cout << sizeof(s) << "Enter stroke: " << endl;
    cin.getline(s, 256);

    cout << "Enter word to find: " << endl;
    cin >> word;

    cout << "Enter word to add: " << endl;
    cin >> wordInsert;

    char* final_out = func(s, word, wordInsert);

    cout << "Final out:" << final_out << endl;

    delete[] final_out;
    final_out = nullptr;

    return 0;
}