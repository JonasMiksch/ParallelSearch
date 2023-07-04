#include "parallelsearch.h"





MyWidget::MyWidget(QWidget* parent) : QWidget(parent) {

    //create widgets;
    m_inputLineEdit = new QLineEdit(this);
    m_button1 = new QPushButton("Linear Search", this);
    m_button2 = new QPushButton("Sorted Search", this);
    m_outputLabel1 = new QLabel(this);
    m_outputLabel2 = new QLabel(this);
    m_outputLabel3 = new QLabel(this);
    m_listWidget = new QListWidget(this);
    m_checkbox = new QCheckBox("Activate search independent from lower/uppercase ", this);
    m_checkbox2 = new QCheckBox("Search in practical list ", this);


    //create layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* topLayout = new QHBoxLayout();
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();
    topLayout->addWidget(m_inputLineEdit);
    rightLayout->addWidget(m_outputLabel1);
    rightLayout->addWidget(m_outputLabel2);
    rightLayout->addWidget(m_outputLabel3);
    rightLayout->addWidget(m_button1);
    rightLayout->addWidget(m_button2);
    rightLayout->addWidget(m_checkbox);
    rightLayout->addWidget(m_checkbox2);
    
    bottomLayout->addWidget(m_listWidget);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addLayout(rightLayout);

    //connect widgets to functions
    connect(m_button1, &QPushButton::clicked, this, &MyWidget::on_button1Clicked);
    connect(m_button2, &QPushButton::clicked, this, &MyWidget::on_button2Clicked);
    connect(m_checkbox, &QCheckBox::stateChanged, this, &MyWidget::handleCheckboxStateChanged1);
    connect(m_checkbox2, &QCheckBox::stateChanged, this, &MyWidget::handleCheckboxStateChanged2);
}
void MyWidget::setTheoreticalList(vector<string> list)
{
    m_list = list;
    m_theoreticalList = list;
    copy(m_list.begin(), m_list.end(), inserter(m_sortedTheoreticalList, m_sortedTheoreticalList.end()));
    m_sortedList = m_sortedTheoreticalList;
}
void MyWidget::setPracticalSet(set<string> list)
{
    m_sortedPracticalList = list;
    vector<string> myVector(m_sortedPracticalList.begin(), m_sortedPracticalList.end());
    m_practicalList = myVector;
}
void MyWidget::handleCheckboxStateChanged1(int state)
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
void MyWidget::handleCheckboxStateChanged2(int state)
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
void MyWidget::on_button1Clicked() {
    QString userInput = m_inputLineEdit->text();
    string input = userInput.toStdString();
    if (userInput == "") {
        m_outputLabel1->setText("Bitte validen Suchstring eingeben");
    }
    else if(input == m_searchString && (!m_lastSearchLinear && !m_toggle)) {

        m_outputLabel1->setText("Das Ergebnis wird bereits angezeigt");
    }
    else {
        m_results.clear();
        m_searchString = input;

        m_outputLabel1->setText("Der Suchstring lautet: " + userInput);

        auto start = std::chrono::high_resolution_clock::now();
        this->multiSearch("linear");
        auto end = std::chrono::high_resolution_clock::now();

        QStringList qStringList;
        for (const auto& result : m_results) {
            qStringList << QString::fromStdString(result);
        }
        m_listWidget->clear();
        m_listWidget->addItems(qStringList);

        m_outputLabel3->setText("Anzahl gefundener Elemente: " + QString::number(m_results.size()));
        m_outputLabel2->setText("Die lineare Suche dauerte " +
            QString::fromStdString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()))) +
            " s");
        m_lastSearchLinear = true;
        m_toggle = false;
    }
}

void MyWidget::on_button2Clicked() {
    QString userInput = m_inputLineEdit->text();
    string input = userInput.toStdString();

    if (userInput == "") {
        m_outputLabel1->setText("Bitte validen Suchstring eingeben");
    }
    else if (input == m_searchString && (!m_lastSearchLinear && !m_toggle)) {
        m_outputLabel1->setText("Das Ergebnis wird bereits angezeigt");
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

        m_outputLabel3->setText("Anzahl gefundener Elemente: " + QString::number(m_results.size()));
        m_outputLabel2->setText("Die sortierte Suche dauerte " +
            QString::fromStdString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()))) +
            " s");
        m_lastSearchLinear = false;
        m_toggle = false;
    }
}

/**
* a normal member taking two arguments and returning an integer value.
* @param a an integer argument.
* @param s a constant character pointer.
* @return The test results
*/

void MyWidget::multiSearch(string type)
{
    const int numThreads = std::thread::hardware_concurrency();
    vector<std::thread> threads;

    vector<string>::iterator start_vec;
    set<string>::iterator start_set;
    int teilliste = 0;

    if (type == "linear") {
        teilliste = m_list.size() / numThreads;
        start_vec = m_list.begin();
    }
    else if (type == "sorted") {
        teilliste = m_sortedList.size()/ numThreads;
        start_set = m_sortedList.begin();
    }

    for (int i = 0; i < numThreads; ++i) {
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



void MyWidget::linearSearch(vector<string>::iterator start, vector<string>::iterator end)
{
    vector<string> localResults;
    for (; start != end; ++start) {
        {
            //check ob das wort größer als der suchstring ist
            if (start->size() >= m_searchString.size()) {
                if (m_caseInsensitive) {
                    string saerchs = m_searchString;
                    if (toLowercase(start->substr(0, m_searchString.size())) == toLowercase(saerchs)) {
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



//mit neuer Liste 648 ms
void MyWidget::setSearch(set<string>::iterator start, set<string>::iterator end) {
    vector<string> localResults;
    set<string>::iterator lower, upper;
    if (m_caseInsensitive) {
        string saerchs = m_searchString;
        lower = lower_bound(start, end, saerchs, [](const std::string& lhs, const std::string& rhs) {
            return toLowercase(lhs) < toLowercase(rhs);
            });
        upper = upper_bound(start, end, saerchs + "\xFF", [](const std::string& lhs, const std::string& rhs) {
            return toLowercase(lhs) < toLowercase(rhs);
            });
    }
    else {
        lower = lower_bound(start, end, m_searchString);
        upper = upper_bound(start, end, m_searchString + "\xFF");
    }

    // Iterate through the range of elements with the leading search string
    for (; lower != upper; ++lower) {
        localResults.push_back(*lower);
    }
    lock_guard<mutex> lock(m_mutex);
    m_results.insert(m_results.end(), localResults.begin(), localResults.end());
}


string toLowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c);
        });
    return result;
}
