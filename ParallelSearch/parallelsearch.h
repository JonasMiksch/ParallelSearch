/*
File: parallelsearch.h
Author : Jonas Miksch
Company : on behalf of Vector Informatik GmbH
Created : 29.06.2023
Modified : 6.07.2023
*/

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
#include<set>
#include<qcheckbox.h>

using namespace std;

class MyWidget : public QWidget {
    Q_OBJECT
private:
    QLineEdit* m_inputLineEdit;

    QPushButton* m_buttonLinear;
    QPushButton* m_buttonSorted;
    QPushButton* m_buttonEvaluate;

    QLabel* m_outputLabel1;
    QLabel* m_outputLabel2;
    QLabel* m_outputLabel3;

    QListWidget* m_listWidget;

    QCheckBox* m_checkbox;
    QCheckBox* m_checkbox2;
    QCheckBox* m_checkbox3;

    vector<string> m_results;
    vector<string> m_list;
    vector<string> m_theoreticalList;
    vector<string> m_practicalList;

    set<string> m_sortedList;
    set<string> m_sortedTheoreticalList;
    set<string> m_sortedPracticalList;

    std::chrono::milliseconds m_timeSet;

    int m_numThreads;

    mutex m_mutex;

    string m_searchString;

    bool m_lastSearchLinear = false;
    bool m_caseInsensitive = false;
    bool m_toggle = false;
    bool m_incrementel = false;


    Ui::ParallelSearchClass ui;
    void multiSearch(string type);
    void linearSearch(vector<string>::iterator start, vector<string>::iterator end);
    void setSearch(set<string>::iterator start, set<string>::iterator end);




private slots:
    void on_button1Clicked();
    void on_button2Clicked();
    void on_button3Clicked();

    void activateCaseInsensitivity(int state);
    void activatePracticalList(int state);
    void activateIncrementalSearch(int state);

    void newInput(const QString& txt);

public:
    void setTheoreticalList(vector<string> list);
    void setPracticalSet(set<string> list);
    MyWidget(QWidget* parent = nullptr);
};
/**
* helper-function for converting a string to lowercase
* @param tring{linear, sorted} sets the type of search algorithm
*/
inline string toLowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c);
        });
    return result;
}