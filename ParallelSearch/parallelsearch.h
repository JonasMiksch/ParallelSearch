#pragma once
#include <QtWidgets/QWidget>
#include "ui_parallelsearch.h"
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <thread>
#include <cassert>
#include <random>
#include<mutex>
#include <chrono>
#include <QLabel>
#include <QListWidget>
using namespace std;

class MyWidget : public QWidget {
    Q_OBJECT
private:
    QLineEdit* inputLineEdit;
    QPushButton* button1;
    QPushButton* button2;
    QPushButton* button3;
    QPushButton* button4;
    QLabel* outputLabel1;
    QLabel* outputLabel2;
    QLabel* outputLabel3;
    QListWidget* listWidget;

    vector<string> m_results;
    vector<string> m_list;
    mutex m_Mutex;

    string m_searchString;
    bool m_liste = false;
    bool m_input = false;
    Ui::ParallelSearchClass ui;
    void rekursivGenerator(string word, int depth);
    void multiSearch();
    

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();
    void onButton4Clicked();

public:
    MyWidget(QWidget* parent = nullptr);
    void generateList();

};
void linearSearch(const vector<string>& wortliste, const string& muster, mutex& rmutex, vector<string>& ergebnisse, int start, int ende);

