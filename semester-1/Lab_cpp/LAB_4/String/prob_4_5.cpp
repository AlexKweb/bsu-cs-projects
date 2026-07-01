#include <iostream>
#include <string>
#include <sstream>


void func(std::string &s,int &word,int &wordInsetr, std::string &s2){
    
    std::stringstream container;// с olipiads.ru
    container.clear();
    container << word;
    std::string wordCopy;
    container >> wordCopy;
    container.clear();

    container.clear();
    container << wordInsetr;
    std::string wordInsertCopy;
    container >> wordInsertCopy;
    container.clear();

    if (s.find(wordCopy) != std::string::npos)
    {
        int pos = s.find(wordCopy);
        s.erase(pos, wordCopy.size());
        s.insert(pos, wordInsertCopy);
    }
    s2 = s;
    //return s2;
}

int main(){
    
    std::cout << "Enter stroke: " << std::endl;
    std::string s;
    getline(std::cin, s);

    std::cout << "Enter int to find: " << std::endl;
    int word;
    std::cin >> word;

    std::cout << "Enter int to add: " << std::endl;
    int wordInsetr;
    std::cin >> wordInsetr;

    std::string s2;
    func(s, word, wordInsetr,s2);

    std::cout << "Final out:" << s2 << std::endl;

    return 0;
}