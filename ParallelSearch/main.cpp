#include "parallelsearch.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{

    QApplication a(argc, argv);
    MyWidget w;
    w.generateList();
    w.show();
    /*
    const vector<string> wortliste = wortlisten_Generator();
    assert(wortliste.size() == pow(26, 4));


    const string muster = "ABC";


    const int numThreads = thread::hardware_concurrency();
    const vector<string> ergebnisse = multi_Suche(numThreads, wortliste, muster);
    std::cout << ergebnisse.size() << std::endl;
    for (auto& ergebnis : ergebnisse) {
        std::cout << ergebnis << std::endl;
    }
    */
    return a.exec();

}
