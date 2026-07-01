#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#pragma warning(disable: 4996)
using namespace std;

// char* func(char* s, char* word, char* wordInsetr) {//qw\0 w

//     if (strstr(s, word) != nullptr) {
//     char* found_ptr = strstr(s, word);

//         size_t index = found_ptr - s;
//         int gap = strlen(wordInsetr);
//         int lenS = strlen(s);
        
//         char tempStr[256]{};

//         for (size_t i = found_ptr - s + strlen(word), k = 0; i < lenS; i++, k++)
//         {   
//             tempStr[k] = s[i];
//         }
//         cout << "1." << tempStr << "<=="<< endl;
//         for (size_t i = found_ptr - s, k=0; i < found_ptr - s + gap +1; i++, k++)
//         {   
//             s[i] = wordInsetr[k];
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

char* func(char* s, char* word, char* wordInsert) {
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
    char s[256], word[256], wordInsert[256];

    cout << sizeof(s) << "Enter stroke: " << endl;
    cin.getline(s, 256);

    cout << "Enter word to find: " << endl;
    cin.getline(word, 256);

    cout << "Enter word to add: " << endl;
    cin.getline(wordInsert, 256);

    char* final_out = func(s, word, wordInsert);

    cout << "Final out:" << final_out << endl << endl;

    delete[] final_out;
    final_out = nullptr;

    return 0;
}
