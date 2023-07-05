/*
  File: parallelsearch.cpp
  Author: Jonas Miksch
  Company: on behalf of Vector Informatik GmbH 
  Created: 29.06.2023
  Modified: 6.07.2023
*/

#include "parallelsearch.h"

MyWidget::MyWidget(QWidget* parent) : QWidget(parent) {

    //create widgets;
    m_inputLineEdit = new QLineEdit(this);
    m_buttonLinear = new QPushButton("Linear Search", this);
    m_buttonSorted = new QPushButton("Sorted Search", this);
    m_buttonEvaluate = new QPushButton("Evaluate speed", this);
    m_outputLabel1 = new QLabel(this);
    m_outputLabel2 = new QLabel(this);
    m_outputLabel3 = new QLabel(this);
    m_listWidget = new QListWidget(this);
    m_checkbox = new QCheckBox("Activate search independent from lower/uppercase ", this);
    m_checkbox2 = new QCheckBox("Search in practical list ", this);
    m_checkbox3 = new QCheckBox("Enable Incremental search ", this);


    //create layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* topLayout = new QHBoxLayout();
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();
    topLayout->addWidget(m_inputLineEdit);
    rightLayout->addWidget(m_outputLabel1);
    rightLayout->addWidget(m_outputLabel2);
    rightLayout->addWidget(m_outputLabel3);
    rightLayout->addWidget(m_buttonLinear);
    rightLayout->addWidget(m_buttonSorted);
    rightLayout->addWidget(m_buttonEvaluate);
    rightLayout->addWidget(m_checkbox);
    rightLayout->addWidget(m_checkbox2);
    rightLayout->addWidget(m_checkbox3);
    
    bottomLayout->addWidget(m_listWidget);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addLayout(rightLayout);

    //connect widgets to functions
    connect(m_buttonLinear, &QPushButton::clicked, this, &MyWidget::on_button1Clicked);
    connect(m_buttonSorted, &QPushButton::clicked, this, &MyWidget::on_button2Clicked);
    connect(m_buttonEvaluate, &QPushButton::clicked, this, &MyWidget::on_button3Clicked);
    connect(m_checkbox, &QCheckBox::stateChanged, this, &MyWidget::activateCaseInsensitivity);
    connect(m_checkbox2, &QCheckBox::stateChanged, this, &MyWidget::activatePracticalList);
    connect(m_checkbox3, &QCheckBox::stateChanged, this, &MyWidget::activateIncrementalSearch);
    connect(m_inputLineEdit, &QLineEdit::textChanged, this, &MyWidget::newInput);

    m_numThreads = std::thread::hardware_concurrency();
}



/**
* search function starting when a new character is typed into input, uses sorted search, enabled by m_checkbox3
* @param txt user set input txt in the qt input line
*/
void MyWidget::newInput(const QString& txt) {
    if (m_incrementel) {
        m_searchString = txt.toStdString();
        m_results.clear();
        this->multiSearch("sorted");
        QStringList qStringList;
        for (const auto& result : m_results) {
            qStringList << QString::fromStdString(result);
        }
        m_listWidget->clear();
        m_listWidget->addItems(qStringList);
        
    }
}
/**
* set m_list as standard search list to theoretical list created in main
* @param list vector<string> list created in main
*/
void MyWidget::setTheoreticalList(vector<string> list)
{
    m_list = list;
    m_theoreticalList = list;
    auto start = std::chrono::high_resolution_clock::now();
    copy(m_list.begin(), m_list.end(), inserter(m_sortedTheoreticalList, m_sortedTheoreticalList.end()));
    auto end = std::chrono::high_resolution_clock::now();
    m_timeSet = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    m_sortedList = m_sortedTheoreticalList;
}
/**
* set list as search list to practical list created in main
* @param list set<string> set created in main
*/
void MyWidget::setPracticalSet(set<string> list)
{
    m_sortedPracticalList = list;
    vector<string> myVector(m_sortedPracticalList.begin(), m_sortedPracticalList.end());
    m_practicalList = myVector;
}
/**
* toggle case insensitivity and m_toggle so the search is performed and not "Result already showing"
* @param state: checked or unchecked
*/
void MyWidget::activateCaseInsensitivity(int state)
{
    if (state == Qt::Checked) {
        m_caseInsensitive = true;
        m_toggle = true;
    }
    else {
        m_caseInsensitive = false;
        m_toggle = true;
    }
}
/**
* toggle between theoretical and practical list
* @param state: checked or unchecked
*/
void MyWidget::activatePracticalList(int state)
{
    if (state == Qt::Checked) {
        
        m_list = m_practicalList;
        m_sortedList = m_sortedPracticalList;
        m_toggle = true;
    }
    else {
        m_list = m_theoreticalList;
        m_sortedList = m_sortedTheoreticalList;
        m_toggle = true;
    }
}
/**
* toggle incremental search
* @param state: checked or unchecked
*/
void MyWidget::activateIncrementalSearch(int state)
{
    if (state == Qt::Checked) {
        m_incrementel = true;
    }
    else {
        m_incrementel = false;
    }
}
/**
* perform linear multithreaded search function on button click
*/
void MyWidget::on_button1Clicked()
{
    QString userInput = m_inputLineEdit->text();
    string input = userInput.toStdString();
    if (userInput == "") {
        m_outputLabel1->setText("Enter valid string");
    }
    else if(input == m_searchString && (m_lastSearchLinear && !m_toggle)) {
        m_outputLabel1->setText("Result already showing");
    }
    else {
        m_results.clear();
        m_searchString = input;

        m_outputLabel1->setText("Searchstring is: " + userInput);

        auto start = std::chrono::high_resolution_clock::now();
        this->multiSearch("linear");
        auto end = std::chrono::high_resolution_clock::now();

        QStringList qStringList;
        for (const auto& result : m_results) {
            qStringList << QString::fromStdString(result);
        }
        m_listWidget->clear();
        m_listWidget->addItems(qStringList);

        m_outputLabel3->setText("Number of found elements: " + QString::number(m_results.size()));
        m_outputLabel2->setText("Linear search took:  " +
            QString::fromStdString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()))) +
            " ms");
        m_lastSearchLinear = true;
        m_toggle = false;
    }
}

/**
* perform sorted  multithreaded search function on button click
*/
void MyWidget::on_button2Clicked()
{
    QString userInput = m_inputLineEdit->text();
    string input = userInput.toStdString();

    if (userInput == "") {
        m_outputLabel1->setText("Enter valid string");
    }
    else if (input == m_searchString && (!m_lastSearchLinear && !m_toggle)) {
        m_outputLabel1->setText("Result already showing");
    }
    else {
        m_results.clear();
        m_searchString = input;

        m_outputLabel1->setText("Der Suchstring lautet: " + userInput);

        auto start = std::chrono::high_resolution_clock::now();
        this->multiSearch("sorted");
        auto end = std::chrono::high_resolution_clock::now();

        QStringList qStringList;
        for (const auto& result : m_results) {
            qStringList << QString::fromStdString(result);
        }
        m_listWidget->clear();
        m_listWidget->addItems(qStringList);

        m_outputLabel3->setText("Number of found elements: " + QString::number(m_results.size()));
        m_outputLabel2->setText("Sorted search took: " +
            QString::fromStdString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()))) +
            "ms");
        m_lastSearchLinear = false;
        m_toggle = false;
    }
}
/**
* evaluate linear and sorted search with a single thread and multiple threads by taking the average of 10 searches
*/
void MyWidget::on_button3Clicked()
{
    QString userInput = m_inputLineEdit->text();
    string input = userInput.toStdString();
    m_results.clear();
    m_searchString = input;

    int bufferThreads = m_numThreads;
    std::chrono::milliseconds totalTimeS(0);
    std::chrono::milliseconds totalTime(0);
    std::chrono::milliseconds totalTimeS1(0);
    std::chrono::milliseconds totalTime1(0);

    for (int i = 0; i < 10; ++i) {
        //sweet spot ~8-12 threads;
        auto startS = std::chrono::high_resolution_clock::now();
        this->multiSearch("sorted");
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds durationS = std::chrono::duration_cast<std::chrono::milliseconds>(endS - startS);
        totalTimeS += durationS;

        m_results.clear();
        auto start = std::chrono::high_resolution_clock::now();
        this->multiSearch("linear");
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        totalTime += duration;
    }

    m_numThreads = 1;

    for (int j = 0; j < 10; ++j) {
        m_results.clear();
        auto startS1 = std::chrono::high_resolution_clock::now();
        this->multiSearch("sorted");
        auto endS1 = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds durationS1 = std::chrono::duration_cast<std::chrono::milliseconds>(endS1 - startS1);
        totalTimeS1 += durationS1;

        m_results.clear();
        auto start1 = std::chrono::high_resolution_clock::now();
        this->multiSearch("linear");
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
        totalTime1 += duration1;
    }
    m_numThreads = bufferThreads;

    totalTime /= 10;
    totalTime1 /= 10;
    totalTimeS /= 10;
    totalTimeS1 /= 10;

    m_outputLabel1->setText("Linear average " +
        QString::fromStdString(std::to_string(totalTime.count())) +
        "ms ||" + "Linear average 1 Thread " +
        QString::fromStdString(std::to_string(totalTime1.count())) +
        "ms ");

    m_outputLabel2->setText("Sorted average " +
        QString::fromStdString(std::to_string(totalTimeS.count())) +
        "ms ||" + "Sorted average 1 Thread " +
        QString::fromStdString(std::to_string(totalTimeS1.count())) +
        "ms ");

    m_outputLabel3->setText("Set creation time " +
        QString::fromStdString(std::to_string(m_timeSet.count())) +
        "ms ");


    m_lastSearchLinear = false;
    m_toggle = false;

}

/**
* multithreaded search function
* @param type string (linear, sorted) sets the type of search algorithm
* @return The test results
*/
void MyWidget::multiSearch(string type)
{  
    vector<std::thread> threads;

    vector<string>::iterator start_vec;
    set<string>::iterator start_set;
    int teilliste = 0;

    if (type == "linear") {
        teilliste = m_list.size() / m_numThreads;
        start_vec = m_list.begin();
    }
    else if (type == "sorted") {
        teilliste = m_sortedList.size()/ m_numThreads;
        start_set = m_sortedList.begin();
    }

    for (int i = 0; i < m_numThreads; ++i) {
        vector<string>::iterator end_vec;
        set<string>::iterator end_set;

        if (type == "linear") {
            end_vec = next(start_vec, teilliste);
        }
        else if (type == "sorted") {
            end_set = next(start_set, teilliste);
        }

        if (type == "linear") {
            threads.emplace_back(&MyWidget::linearSearch, this, start_vec, end_vec);
            start_vec = end_vec;
        }
        else if(type=="sorted"){
            threads.emplace_back(&MyWidget::setSearch, this, start_set, end_set);
            start_set = end_set;
        }
        
    }
    for (auto& thread : threads) {
        thread.join();
    }
}


/**
* linear search function
* @param start iterator to the start of the area of interest
* @param end iterator to the end of the area of interest
*/
void MyWidget::linearSearch(vector<string>::iterator start, vector<string>::iterator end)
{
    vector<string> localResults;
    for (; start != end; ++start) {
        {
            //check ob das wort größer als der suchstring ist
            if (start->size() >= m_searchString.size()) {
                if (m_caseInsensitive) {
                    string searchS = m_searchString;
                    if (toLowercase(start->substr(0, m_searchString.size())) == toLowercase(searchS)) {
                        localResults.push_back(*start);
                    }
                }
                else {
                    if (start->substr(0, m_searchString.size()) == m_searchString) {
                        localResults.push_back(*start);
                    }
                }
                
            }
        }
    }
    lock_guard<mutex> lock(m_mutex);
    m_results.insert(m_results.end(), localResults.begin(), localResults.end());
}


/**
* sorted search function
* @param start iterator to the start of the area of interest
* @param end iterator to the end of the area of interest
*/
void MyWidget::setSearch(set<string>::iterator start, set<string>::iterator end) {
    set<string> localResults;
    set<string>::iterator lower, upper;
    if (m_caseInsensitive) {
        string searchS = m_searchString;
        lower = lower_bound(start, end, searchS, [](const std::string& lhs, const std::string& rhs) {
            return toLowercase(lhs) < toLowercase(rhs);
            });
        upper = upper_bound(start, end, searchS + "\xFF", [](const std::string& lhs, const std::string& rhs) {
            return toLowercase(lhs) < toLowercase(rhs);
            });
    }
    else {
        lower = lower_bound(start, end, m_searchString);
        upper = upper_bound(start, end, m_searchString + "\xFF");
    }

    for (; lower != upper; ++lower) {
        localResults.insert(*lower);
    }
    lock_guard<mutex> lock(m_mutex);
    m_results.insert(m_results.end(), localResults.begin(), localResults.end());
}


