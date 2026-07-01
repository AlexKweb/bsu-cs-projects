#include <iostream>
#include <cstring>

using namespace std;

char* func(char* s,char* word){
    if (strstr(s, word) != nullptr)
    {
        int index = strstr(s, word) - s;
        for (size_t i = index; i < index + strlen(word); i++)
        {
            s[i]= ' ';
        }   
    }
    for (char c : {' ',',', '.', '!', '?', ';', ':'}) {
        bool toggle = true;
        char pattern[3] = {' ', c, '\0'};
        while(toggle){
            if (strstr(s, pattern)!= nullptr)
            {
                // cout << " work. ";
                size_t index = strstr(s, pattern) - s;
                for (size_t i = index; i <= strlen(s); i++)
                {
                    s[i]= s[i+1];
                }     
            }else toggle = false;
        }
    }
    return s;
}

int main(){
    
    cout << "Enter stroke: " << endl;
    char* s = new char[256];
    cin.getline(s, 256);

    cout << "Enter word to delete: " << endl;
    char* word = new char[256];
    cin.getline(word, 256);

    cout << "Final out:" << func(s, word) << endl << endl;

    return 0;
}