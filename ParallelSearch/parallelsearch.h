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
    QPushButton* m_button1;
    QPushButton* m_button2;

    QLabel* m_outputLabel1;
    QLabel* m_outputLabel2;
    QLabel* m_outputLabel3;
    QListWidget* m_listWidget;
    QCheckBox* m_checkbox;
    QCheckBox* m_checkbox2;

    vector<string> m_results;
    vector<string> m_list;
    vector<string> m_theoreticalList;
    vector<string> m_practicalList;
    mutex m_mutex;
    set<string> m_sortedList;
    set<string> m_sortedTheoreticalList;
    set<string> m_sortedPracticalList;

    string m_searchString;
    bool m_lastSearchLinear = false;
    bool m_caseInsensitive = false;
    bool m_toggle = false;
    Ui::ParallelSearchClass ui;
    void multiSearch(string type);

    void linearSearch(vector<string>::iterator start, vector<string>::iterator end);
    void setSearch(set<string>::iterator start, set<string>::iterator end);




private slots:
    void on_button1Clicked();
    void on_button2Clicked();

    void handleCheckboxStateChanged1(int state);
    void handleCheckboxStateChanged2(int state);

public:
    void setTheoreticalList(vector<string> list);
    void setPracticalSet(set<string> list);
    MyWidget(QWidget* parent = nullptr);

};
string toLowercase(const string& str);