#include <iostream>
#include <string>

void func(std::string &s,std::string &word,std::string &wordInsetr, std::string &s2){
    if (s.find(word) != std::string::npos)
    {
        int pos = s.find(word);
        s.erase(pos, word.length());
        s.insert(pos, wordInsetr);
    }
    s2 = s;
    //return s2;
}

int main(){
    
    std::cout << "Enter stroke: " << std::endl;
    std::string s;
    getline(std::cin, s);

    std::cout << "Enter word to find: " << std::endl;
    std::string word;
    getline(std::cin, word);

    std::cout << "Enter word to add: " << std::endl;
    std::string wordInsetr;
    getline(std::cin, wordInsetr);

    std::string s2;
    func(s, word, wordInsetr,s2);

    std::cout << "Final out:" << s2 << std::endl;

    return 0;
}