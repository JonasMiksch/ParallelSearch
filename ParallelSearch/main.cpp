#include "parallelsearch.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
std::string filename = "C:\\Jonas\\Coden\\Vector_JM\\ParallelSearch\\mit.edu_~ecprice_wordlist.10000.txt";

void rekursivGenerator(vector<string>& list, string word, int depth) {
    if (depth == 4) {
        list.push_back(word);
        return;
    }
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        string newWord = word + letter;
        rekursivGenerator(list, newWord, depth + 1);
    }
}
vector<string> generateList() {
    vector<string> list;
    rekursivGenerator(list, "", 0);

    //shuffle um den Zufall zu gewährleisten
    auto rng = default_random_engine{};
    shuffle(begin(list), end(list), rng);

    assert(list.size() == pow(26, 4));
    return list;
}
set<string> loadList() {
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
{

    QApplication a(argc, argv);
    MyWidget w;
    w.setPracticalSet(loadList());
    w.setTheoreticalList(generateList());
    w.show();

    return a.exec();

}

