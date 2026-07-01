#include <iostream>
#include <string>

std::string func(std::string &s,std::string &word,std::string &wordInsetr){
    if (s.find(word) != std::string::npos)
    {
        s.insert(s.find(word) + word.size(), " "+wordInsetr);
    }
    return s;
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

    std::cout << "Final out:" << func(s, word, wordInsetr) << std::endl;

    return 0;
}