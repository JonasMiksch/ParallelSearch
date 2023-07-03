#include "parallelsearch.h"



MyWidget::MyWidget(QWidget* parent) : QWidget(parent) {
    inputLineEdit = new QLineEdit(this);
    button1 = new QPushButton("Lineare Suche", this);
    button2 = new QPushButton("Button 2", this);
    button3 = new QPushButton("Button 3", this);
    button4 = new QPushButton("Button 4", this);

    button2->setEnabled(true);
    button3->setEnabled(false);
    button4->setEnabled(false);

    outputLabel1 = new QLabel(this);
    outputLabel2 = new QLabel(this);
    outputLabel3 = new QLabel(this);

    listWidget = new QListWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* topLayout = new QHBoxLayout();
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();

    topLayout->addWidget(inputLineEdit);
    rightLayout->addWidget(button1);
    rightLayout->addWidget(button2);
    rightLayout->addWidget(button3);
    rightLayout->addWidget(button4);
    bottomLayout->addWidget(outputLabel1);
    bottomLayout->addWidget(outputLabel2);
    bottomLayout->addWidget(outputLabel3);
    bottomLayout->addWidget(listWidget);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addLayout(rightLayout);

    connect(button1, &QPushButton::clicked, this, &MyWidget::onButton1Clicked);
    connect(button2, &QPushButton::clicked, this, &MyWidget::onButton2Clicked);
    connect(button3, &QPushButton::clicked, this, &MyWidget::onButton3Clicked);
    connect(button4, &QPushButton::clicked, this, &MyWidget::onButton4Clicked);
}

void MyWidget::onButton1Clicked() {
    QString userInput = inputLineEdit->text();
    string input = userInput.toStdString();
    if (userInput == "") {
        outputLabel1->setText("Bitte validen Suchstring eingeben");
    }
    else if(input == m_searchString) {
        outputLabel1->setText("Das Ergebnis wird bereits angezeigt");
    }
    else {
        m_results.clear();
        m_searchString = input;

        outputLabel1->setText("Der Suchstring lautet: " + userInput);

        auto start = std::chrono::high_resolution_clock::now();
        this->multiSearch();
        auto end = std::chrono::high_resolution_clock::now();

        QStringList qStringList;
        for (const auto& result : m_results) {
            qStringList << QString::fromStdString(result);
        }
        listWidget->clear();
        listWidget->addItems(qStringList);

        outputLabel3->setText("Anzahl gefundener Elemente: " + QString::number(m_results.size()));
        outputLabel2->setText("Die lineare Suche dauerte " +
            QString::fromStdString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()))) +
            " s");
    }
}

void MyWidget::onButton2Clicked() {

}

void MyWidget::onButton3Clicked() {
    
}

void MyWidget::onButton4Clicked() {
    outputLabel2->setText("Button 4 wurde geklickt");
}

void MyWidget::rekursivGenerator(string word, int depth) {
    if (depth == 4) {
        m_list.push_back(word);
        return;
    }
    for (char buchstabe = 'A'; buchstabe <= 'Z'; ++buchstabe) {
        string newWord = word + buchstabe;
        rekursivGenerator(newWord, depth + 1);
    }
}
void MyWidget::generateList() {

    rekursivGenerator("", 0);

    //shuffle um den Zufall zu gewährleisten
    auto rng = default_random_engine{};
    shuffle(begin(m_list), end(m_list), rng);

    assert(m_list.size() == pow(26, 4));
}

void MyWidget::multiSearch()
{
    const int numThreads = std::thread::hardware_concurrency();
    vector<std::thread> threads;
    
    int teilliste = m_list.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * teilliste;
        int ende;
        if (i != numThreads - 1) {
            ende = (i + 1) * teilliste - 1;
        }
        else {
            ende = m_list.size() - 1;
        }
        //funktioniert nur mit ref
        threads.emplace_back(linearSearch, m_list, m_searchString, ref(m_Mutex), ref(m_results), start, ende);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void linearSearch(const vector<string>& wortliste, const string& muster, mutex& rmutex, vector<string>& ergebnisse, int start, int ende)
{
    vector<string> lokaleErgebnisse;
    for (int i = start; i <= ende; ++i)
    {
        //check ob das wort größer als der suchstring ist
        if (wortliste[i].size() >= muster.size()) {
            if (wortliste[i].substr(0, muster.size()) == muster) {
                lokaleErgebnisse.push_back(wortliste[i]);
            }
        }
    }
    lock_guard<mutex> lock(rmutex);
    ergebnisse.insert(ergebnisse.end(), lokaleErgebnisse.begin(), lokaleErgebnisse.end());
}
