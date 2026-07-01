#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

// char* func(char* s, char* word, char* wordInsetr) {//qw\0 w

//     if (strstr(s, word) != nullptr) {
//     char* found_ptr = strstr(s, word)+strlen(word);

//         size_t index = found_ptr - s;
//         int gap = strlen(wordInsetr);
//         int lenS = strlen(s);
        
//         for (size_t i = strlen(s) - 1 - gap ; i < lenS; i++)
//         {   
//             s[i+gap+1] = s[i+1];
//         }
//         for (size_t i = strlen(s)-1; i >= found_ptr - s + gap; i--)
//         {   
//             s[i] = s[i-gap];
//         }
//         for (size_t i = found_ptr - s, k = 0; i < found_ptr - s + gap; i++, k++)
//         {
//             s[i] = wordInsetr[k];
//         }
//     }
//     return s;
// }
char* func(char* s, const char* word, const char* wordInsert) {
    char* pos = strstr(s, word);
    if (!pos) return s;

    size_t wordLen = strlen(word);
    size_t insertLen = strlen(wordInsert);
    size_t tailLen = strlen(pos + wordLen);

    memmove(pos + wordLen + insertLen, pos + wordLen, tailLen + 1);
    memcpy(pos + wordLen, wordInsert, insertLen);

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

    cout << "Final out:" << final_out << endl;

    delete[] final_out;
    final_out = nullptr;

    return 0;
}
