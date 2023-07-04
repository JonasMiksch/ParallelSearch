#include "parallelsearch.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
std::string filename = "C:\\Jonas\\Coden\\Vector_JM\\ParallelSearch\\mit.edu_~ecprice_wordlist.10000.txt";

void recursiveGenerator(vector<string>& list, string word, int depth)
/**
* rekursiv helper function to create all words from AAAA to ZZZZ
* @param list: the resulting list
* @param word: the newly created word up to depth
* @param depth: number of characters added to the word;0-4
*/
{
    if (depth == 4) {
        list.push_back(word);
        return;
    }
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        string newWord = word + letter;
        recursiveGenerator(list, newWord, depth + 1);
    }
}
vector<string> generateList() 
/**
* generate a list for the search algorithm containing all words from AAAA to ZZZZ
*/
{
    vector<string> list;
    recursiveGenerator(list, "", 0);

    //shuffle um den Zufall zu gewährleisten
    auto rng = default_random_engine{};
    shuffle(begin(list), end(list), rng);

    assert(list.size() == pow(26, 4));
    return list;
}
set<string> loadList()
/**
* load a list for the search algorithm specified in global parameter filename
*/
{
    fstream file(filename, ios::in);
    string line, word;
    set<string> words;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::istringstream stream(line);
            std::string word;
            while (stream >> word) {
                words.insert(word);
            }
        }
        file.close();
    }
    else {
        cout << "File could not be openend";
    }
    return words;
}
int main(int argc, char* argv[])
/**
* load and set lists, create GUI for the search algoritms
*/
{

    QApplication a(argc, argv);
    MyWidget w;
    w.setPracticalSet(loadList());
    w.setTheoreticalList(generateList());
    w.show();

    return a.exec();

}

