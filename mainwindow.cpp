#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    tabs[0] = &MainWindow::printA;
    tabs[1] = &MainWindow::printB;
    tabs[2] = &MainWindow::printC;
    tabs[3] = &MainWindow::printD;
    tabs[4] = &MainWindow::printE;
    tabs[5] = &MainWindow::printF;
    tabs[6] = &MainWindow::printG;
    tabs[7] = &MainWindow::printH;
    tabs[8] = &MainWindow::printI;
    tabs[9] = &MainWindow::printJ;
    tabs[10] = &MainWindow::printK;
    tabs[11] = &MainWindow::printL;
    tabs[12] = &MainWindow::printM;
    tabs[13] = &MainWindow::printN;
    tabs[14] = &MainWindow::printO;
    tabs[15] = &MainWindow::printP;
    tabs[16] = &MainWindow::printQ;
    tabs[17] = &MainWindow::printR;
    tabs[18] = &MainWindow::printS;
    tabs[19] = &MainWindow::printT;
    tabs[20] = &MainWindow::printU;
    tabs[21] = &MainWindow::printV;
    tabs[22] = &MainWindow::printW;
    tabs[23] = &MainWindow::printX;
    tabs[24] = &MainWindow::printY;
    tabs[25] = &MainWindow::printZ;
    tabs[26] = &MainWindow::printSummary;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("text files (*.txt)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        QString line;
        if(file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream in(&file);
            line = in.readAll();
            if(treeButton->isChecked())
                processWithTree(line);
            else if(heapButton->isChecked())
                processWithHeap(line);
            else
                printSummary("You must select \"Tree\" or \"Heap\"");
        }
        else
            printSummary("Error in opening " + fileName);
        file.close();
    }
}

void MainWindow::processWithHeap(QString textFiles)
{
    Heap x;
    string textFile = textFiles.toStdString();
    QTime t;
    t.start();

    int totalPar, totalSentences = 1;

    char *str = new char[50];
    char next;
    int par = 1, sentence = 1;
    bool isQuote = false;

    int i = 0;
    int fileCount = 0;
    next = textFile[fileCount];
    int fileLength = textFile.length();
    while(fileCount < fileLength)
    {
        while(isalpha(next) || next == '-')
        {
            getPhrase(str, tolower(next), i);
            if(fileCount<fileLength)
                next = textFile[++fileCount];
            if(next == '-' || next == '\'')
            {
                char adjacent;
                if(fileCount < fileLength)
                    adjacent = textFile[++fileCount];
                if(!isalpha(adjacent))
                    next = '$';    //exit while loop
                else
                {
                    getPhrase(str, tolower(next), i);
                    next = adjacent;
                }
            }
        }
        str[i] = '\0';
        if(i != 0)
        {
            str[0] = toupper(str[0]);
            x.insert(str, par, sentence);
        }
        while(!isalpha(next) && fileCount < fileLength)
        {
            if(next == '\n')
            {
                par++;
                while(next == '\n' && fileCount < fileLength)
                {
                    next = textFile[++fileCount];
                }
                totalSentences++;
                sentence = 1;
            }
            else if(next == '"')
            {
                if(!isQuote)
                {
                    isQuote = true;
                    next = textFile[++fileCount];
                }
                else
                {
                    isQuote = false;
                    next = textFile[++fileCount];
                    if(next == ' ')
                    {
                        if(fileCount<fileLength)
                            next = textFile[++fileCount];
                        if(!islower(next))
                        {
                            sentence++;
                            totalSentences++;
                        }
                    }
                }
            }
            else if((next == '.' || next == '!' || next == '?') && !isQuote)
            {
                next = textFile[++fileCount];
                if(next == ' ')
                {
                    if(fileCount<fileLength)
                        next = textFile[++fileCount];
                    if(!islower(next))
                    {
                        sentence++;
                        totalSentences++;
                    }
                }
            }
            else
            {
                next = textFile[++fileCount];
            }
        }
        if(next == '\n')
        {
            totalPar = par-1;
            totalSentences--;
        }
        else
            totalPar = par;
        i = 0;
    }
    printSummary("Time to place into heap " + QString::number(t.elapsed()/1000.0) + " seconds");


    //-----------------------output-------------------
    int totalRepeatedWords = x.numOfElements();
    Node *mostFreq = new Node[10];
    int mostFreqSize = 0;
    int *startLetterFreq = new int[26];
    for(int i = 0; i < 26; i++)
        startLetterFreq[i] = 0;
    int j = 1;
    int totalWords = x.numOfElements();
    (this->*tabs[26])("Total Words: " + QString::number(totalWords));
    Node prev = x.removeMin();
    int totalSyllables = 0;
    while(j <= totalWords)
    {
        int count = 1;
        Node next = x.removeMin();
        (this->*tabs[(prev.data)[0]-'A'])(QString::number(prev.par) + ", " + QString::number(prev.line));
        j++;
        while(prev.data == next.data && j <= totalWords)
        {
            (this->*tabs[(prev.data)[0]-'A'])(QString::number(next.par) + ", " + QString::number(next.line));
            count++;
            next = x.removeMin();
            j++;
        }
        if(mostFreqSize < 10)
        {
            getNode(mostFreq, prev.data, count, mostFreqSize);
            sortAscendingNode(mostFreq, mostFreqSize);
        }
        else
        {
            findMostFreq(mostFreq, 10, prev.data, count);
        }

        if(prev.data.length() < 3)
            totalSyllables += count;
        else
            totalSyllables += prev.data.length()/3 * count;

        (this->*tabs[(prev.data)[0]-'A'])(QString::number(count) + " " + QString::fromStdString(prev.data));

        increaseFreq(startLetterFreq, (prev.data)[0]-'A');
        prev = next;

    }
    (this->*tabs[26])("Most frequent words: ");
    for(int i = mostFreqSize-1; i >= 0 ; i--)
        (this->*tabs[26])(QString::number(mostFreq[i].count) + " " + QString::fromStdString(mostFreq[i].data));

    (this->*tabs[26])("Number of words that start with: ");
    for(int i = 0; i < 26; i++)
        (this->*tabs[26])((QString)(QChar)(65+i) + ": " + QString::number(startLetterFreq[i]));

    (this->*tabs[26])("Number of paragraphs: " + QString::number(totalPar));
    (this->*tabs[26])("Total sentences: " + QString::number(totalSentences));
    (this->*tabs[26])("Total syllables: " + QString::number(totalSyllables));
    double readingLevel = 0.39*(totalRepeatedWords/totalSentences) + 11.8*(totalSyllables/totalRepeatedWords) - 15.59;
    (this->*tabs[26])("Reading level: " + QString::number(readingLevel));
    (this->*tabs[26])("Time to process document: " + QString::number(t.elapsed()/1000.0) + " seconds");
}

void MainWindow::processWithTree(QString textFiles)
{
    Tree x;
    string textFile = textFiles.toStdString();
    QTime t;
    t.start();

    int totalPar, totalSentences = 1;

    char *str = new char[50];
    char next;
    int par = 1, sentence = 1;
    bool isQuote = false;

    int i = 0;
    int fileCount = 0;
    next = textFile[fileCount];
    int fileLength = textFile.length();
    while(fileCount < fileLength)
    {
        while(isalpha(next) || next == '-')
        {
            getPhrase(str, tolower(next), i);
            if(fileCount<fileLength)
                next = textFile[++fileCount];
            if(next == '-' || next == '\'')
            {
                char adjacent;
                if(fileCount < fileLength)
                    adjacent = textFile[++fileCount];
                if(!isalpha(adjacent))
                    next = '$';    //exit while loop
                else
                {
                    getPhrase(str, tolower(next), i);
                    next = adjacent;
                }
            }
        }
        str[i] = '\0';
        if(i != 0)
        {
            str[0] = toupper(str[0]);
            x.insert(str, par, sentence);
        }
        while(!isalpha(next) && fileCount < fileLength)
        {
            if(next == '\n')
            {
                par++;
                while(next == '\n' && fileCount < fileLength)
                {
                    next = textFile[++fileCount];
                }
                totalSentences++;
                sentence = 1;
            }
            else if(next == '"')
            {
                if(!isQuote)
                {
                    isQuote = true;
                    next = textFile[++fileCount];
                }
                else
                {
                    isQuote = false;
                    next = textFile[++fileCount];
                    if(next == ' ')
                    {
                        if(fileCount<fileLength)
                            next = textFile[++fileCount];
                        if(!islower(next))
                        {
                            sentence++;
                            totalSentences++;
                        }
                    }
                }
            }
            else if((next == '.' || next == '!' || next == '?') && !isQuote)
            {
                next = textFile[++fileCount];
                if(next == ' ')
                {
                    if(fileCount<fileLength)
                        next = textFile[++fileCount];
                    if(!islower(next))
                    {
                        sentence++;
                        totalSentences++;
                    }
                }
            }
            else
            {
                next = textFile[++fileCount];
            }
        }
        if(next == '\n')
        {
            totalPar = par-1;
            totalSentences--;
        }
        else
            totalPar = par;
        i = 0;
        //fileCount++;
    }
    printSummary("Time to place into tree " + QString::number(t.elapsed()/1000.0) + " seconds");

    //----------------output---------------------
    int totalWords = x.numOfElements();
    NodeT *storage = new NodeT[totalWords];
    int count = 0;
    x.obtainElementsIN(storage, count, 0);
    NodeT *mostFreq = new NodeT[10];
    int mostFreqSize = 0;
    int *startLetterFreq = new int[26];
    for(int i = 0; i < 26; i++)
        startLetterFreq[i] = 0;
    int totalRepeatedWords = 0;
    int j = 0;

    (this->*tabs[26])("Total Words: " + QString::number(totalWords));

    int totalSyllables = 0;
    while(j < totalWords)
    {
        if(mostFreqSize < 10)
        {
            getNode(mostFreq, storage[j].data, storage[j].count, mostFreqSize);
            sortAscendingNode(mostFreq, mostFreqSize);
        }
        else
        {
            findMostFreq(mostFreq, 10, storage[j].data, storage[j].count);
        }

        if((storage[j].data).length() < 3)
            totalSyllables += storage[j].count;
        else
            totalSyllables += (storage[j].data).length()/3 * storage[j].count;

        totalRepeatedWords += storage[j].count;
        int tabNumber = (storage[j].data)[0]-'A';
        (this->*tabs[tabNumber])(QString::number(storage[j].count) + " " + QString::fromStdString(storage[j].data));

        for(int i = 0; i < storage[j].count; i++)
            (this->*tabs[tabNumber])(QString::number(storage[j].location[i].par) + ", " + QString::number(storage[j].location[i].line));
        increaseFreq(startLetterFreq, (storage[j].data)[0]-'A');
        j++;

    }
    (this->*tabs[26])("All words: " + QString::number(totalRepeatedWords) );
    (this->*tabs[26])("Most frequent words: ");
    for(int i = mostFreqSize-1; i >= 0 ; i--)
        (this->*tabs[26])(QString::number(mostFreq[i].count) + " " + QString::fromStdString(mostFreq[i].data));
    (this->*tabs[26])("Number of words that start with: ");
    for(int i = 0; i < 26; i++)
        (this->*tabs[26])(((QString)(QChar)(65+i)) + ": " + QString::number(startLetterFreq[i]));
    (this->*tabs[26])("Number of paragraphs: " + QString::number(totalPar));
    (this->*tabs[26])("Total sentences: " + QString::number(totalSentences));
    (this->*tabs[26])("Total syllables: " + QString::number(totalSyllables));
    double readingLevel = 0.39*(totalRepeatedWords/totalSentences) + 11.8*(totalSyllables/totalRepeatedWords) - 15.59;
    (this->*tabs[26])("Reading level: " + QString::number(readingLevel));

    (this->*tabs[26])("Time to process document: " + QString::number(t.elapsed()/1000.0) + " seconds");


}

void MainWindow::printA(QString x)
{
    textBrowser_1->append(x);
}

void MainWindow::printB(QString x)
{
    textBrowser_2->append(x);
}

void MainWindow::printC(QString x)
{
    textBrowser_3->append(x);
}

void MainWindow::printD(QString x)
{
    textBrowser_4->append(x);
}

void MainWindow::printE(QString x)
{
    textBrowser_5->append(x);
}

void MainWindow::printF(QString x)
{
    textBrowser_6->append(x);
}

void MainWindow::printG(QString x)
{
    textBrowser_7->append(x);
}

void MainWindow::printH(QString x)
{
    textBrowser_8->append(x);
}

void MainWindow::printI(QString x)
{
    textBrowser_9->append(x);
}

void MainWindow::printJ(QString x)
{
    textBrowser_10->append(x);
}

void MainWindow::printK(QString x)
{
    textBrowser_11->append(x);
}
void MainWindow::printL(QString x)
{
    textBrowser_12->append(x);
}

void MainWindow::printM(QString x)
{
    textBrowser_13->append(x);
}

void MainWindow::printN(QString x)
{
    textBrowser_14->append(x);
}

void MainWindow::printO(QString x)
{
    textBrowser_15->append(x);
}

void MainWindow::printP(QString x)
{
    textBrowser_16->append(x);
}
void MainWindow::printQ(QString x)
{
    textBrowser_17->append(x);
}

void MainWindow::printR(QString x)
{
    textBrowser_18->append(x);
}

void MainWindow::printS(QString x)
{
    textBrowser_19->append(x);
}

void MainWindow::printT(QString x)
{
    textBrowser_20->append(x);
}

void MainWindow::printU(QString x)
{
    textBrowser_21->append(x);
}
void MainWindow::printV(QString x)
{
    textBrowser_22->append(x);
}

void MainWindow::printW(QString x)
{
    textBrowser_23->append(x);
}

void MainWindow::printX(QString x)
{
    textBrowser_24->append(x);
}

void MainWindow::printY(QString x)
{
    textBrowser_25->append(x);
}

void MainWindow::printZ(QString x)
{
    textBrowser_26->append(x);
}

void MainWindow::printSummary(QString x)
{
    textBrowser->append(x);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getPhrase(char *phrase, char x, int &size)
{
    phrase[size] = x;
    size++;
}

void MainWindow::getNode(Node *x, string str, int count, int &size)
{
    x[size].count = count;
    x[size].data = str;
    size++;
}

void MainWindow::sortAscendingNode(Node *x, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = i+1; j < size; j++)
        {
            if(x[i].count > x[j].count)
            {
                Node temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }
}

void MainWindow::findMostFreq(Node *x, int size, string str, int count)
{
    if(count > x[0].count)
    {
        x[0].count = count;
        x[0].data = str;
        int i = 0;
        while(x[i].count > x[i+1].count && i < size-1)
        {
            Node temp = x[i];
            x[i] = x[i+1];
            x[i+1] = temp;
            i++;
        }
    }
}

void MainWindow::increaseFreq(int *array, int index)
{
    array[index] += 1;
}

void MainWindow::getNode(NodeT *x, string str, int count, int &size)
{
    x[size].count = count;
    x[size].data = str;
    size++;
}

void MainWindow::sortAscendingNode(NodeT *x, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = i+1; j < size; j++)
        {
            if(x[i].count > x[j].count)
            {
                NodeT temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }
}

void MainWindow::findMostFreq(NodeT *x, int size, string str, int count)
{
    if(count > x[0].count)
    {
        x[0].count = count;
        x[0].data = str;
        int i = 0;
        while(x[i].count > x[i+1].count && i < size-1)
        {
            NodeT temp = x[i];
            x[i] = x[i+1];
            x[i+1] = temp;
            i++;
        }
    }
}
