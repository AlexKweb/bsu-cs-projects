#include <iostream>
#include <string>

using namespace std;

string func(string &s,string &word){
    if (s.find(word) != string::npos)
    {
        // if (s.find(word) == 0)
        // {
        //     s.erase(s.find(word), word.length()+1);
        //     return s;
        // }
        s.erase(s.find(word), word.length()+1);
    }
    // if ((s.find("  ")!= string::npos) 
    //     || (s.find(" ,")!= string::npos) 
    //     || (s.find(" .")!= string::npos))
    for (char c : {' ',',', '.', '!', '?', ';', ':'}) {
        string pattern = " ";
        pattern += c;
        if (s.find(pattern) != string::npos) {
            s.erase(s.find(pattern), 1);
        }
    }
    return s;
}

int main(){
    
    cout << "Enter stroke: " << endl;
    string s;
    getline(cin, s);

    cout << "Enter word to delete: " << endl;
    string word;
    getline(cin, word);

    cout << "Final out:" << func(s, word) << endl << endl;

    return 0;
}