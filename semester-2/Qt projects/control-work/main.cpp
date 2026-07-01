#include <QCoreApplication>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QString>
#include <algorithm>

struct NameTel
{
    QString name;
    long long tel1;
    long long tel2;
};

struct NameAddr
{
    QString name;
    QString addr;
    long long tel1;
};

// --------------------

QTextStream& operator>>(QTextStream& in, NameTel& rhs)
{
    in >> rhs.name >> rhs.tel1 >> rhs.tel2;
    return in;
}

QTextStream& operator<<(QTextStream& out, const NameTel& rhs)
{
    out << rhs.name << ' ' << rhs.tel1 << ' ' << rhs.tel2 << Qt::endl;
    return out;
}

QDataStream& operator>>(QDataStream& in, NameTel& rhs)
{
    in >> rhs.name >> rhs.tel1 >> rhs.tel2;
    return in;
}

QDataStream& operator<<(QDataStream& out, const NameTel& rhs)
{
    out << rhs.name << rhs.tel1 << rhs.tel2;
    return out;
}

// --------------------

QTextStream& operator>>(QTextStream& in, NameAddr& rhs)
{
    in >> rhs.name >> rhs.addr >> rhs.tel1;
    return in;
}

QTextStream& operator<<(QTextStream& out, const NameAddr& rhs)
{
    out << rhs.name << ' ' << rhs.addr << ' ' << rhs.tel1 << Qt::endl;
    return out;
}

QDataStream& operator>>(QDataStream& in, NameAddr& rhs)
{
    in >> rhs.name >> rhs.addr >> rhs.tel1;
    return in;
}

QDataStream& operator<<(QDataStream& out, const NameAddr& rhs)
{
    out << rhs.name << rhs.addr << rhs.tel1;
    return out;
}

// --------------------

template <typename T>
class Solution
{
private:
    QVector<T> data;

public:
    Solution(){};

    Solution(const QVector<T>& otherData): data(otherData){};

    Solution(const Solution<T>& other): data(other.data){};

    Solution(QTextStream& in)
    {
        readText(in);
    }

    Solution(QDataStream& in)
    {
        readBin(in);
    }
    void readText(QTextStream& in)
    {
        data.clear();

        while (!in.atEnd()) {
            T temp;
            in >> temp;

            if (in.atEnd())
                break;

            data.push_back(temp);
        }
    }

    void writeText(QTextStream& out)
    {
        for (int i = 0; i < data.size(); i++) {
            out << data[i];
        }
    }

    void writeBin(QDataStream& out)
    {
        int n = data.size();
        out << n;

        for (int i = 0; i < data.size(); i++) {
            out << data[i];
        }
    }

    void readBin(QDataStream& in)
    {
        data.clear();

        int n;
        in >> n;

        for (int i = 0; i < n; i++) {
            T temp;
            in >> temp;
            data.push_back(temp);
        }
    }

    void sortByName()
    {
        std::sort(data.begin(), data.end(),
                  [](const T& a, const T& b) {
                      return a.name < b.name;
                  });
    }

    void sortByTel1()
    {
        std::sort(data.begin(), data.end(),
                  [](const T& a, const T& b) {
                      return a.tel1 < b.tel1;
                  });
    }

    std::pair<int, int> findByName(QString targetName)
    {
        auto left = std::lower_bound(
            data.begin(),
            data.end(),
            targetName,
            [](const T& a, const QString& name) {
                return a.name < name;
            }
            );

        auto right = std::upper_bound(
            data.begin(),
            data.end(),
            targetName,
            [](const QString& name, const T& a) {
                return name < a.name;
            }
            );

        return {int(left - data.begin()), int(right - data.begin())};
    }

    T& operator[](int i)
    {
        return data[i];
    }

    const T& operator[](int i) const
    {
        return data[i];
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cout(stdout);

    QFile outputFile("output.txt");
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Can't open output.txt" << Qt::endl;
        return 1;
    }
    QTextStream out(&outputFile);

    // NameTel
    QFile fileTelTxt("nametel.txt");
    if (!fileTelTxt.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Can't open nametel.txt" << Qt::endl;
        return 1;
    }
    cout << "nametel.txt opened" << Qt::endl;

    QTextStream inTelTxt(&fileTelTxt);
    Solution<NameTel> telBook;
    telBook.readText(inTelTxt);
    fileTelTxt.close();

    out << "========== NameTel ==========" << Qt::endl;
    out << "NameTel from txt:" << Qt::endl;
    telBook.writeText(out);
    out << "-------------------" << Qt::endl;

    telBook.sortByName();
    out << "Sorted by name:" << Qt::endl;
    telBook.writeText(out);
    out << "-------------------" << Qt::endl;

    std::pair<int, int> range1 = telBook.findByName("Ivan");
    if (range1.first != range1.second) {
        out << "Found NameTel range:" << Qt::endl;
        for (int i = range1.first; i < range1.second; i++) {
            out << telBook[i];
        }
    }
    else {
        cout << "NameTel binary search completed: Ivan not found" << Qt::endl;
        out << "NameTel not found" << Qt::endl;
    }
    out << "-------------------" << Qt::endl;

    telBook.sortByTel1();
    out << "Sorted by tel1:" << Qt::endl;
    telBook.writeText(out);
    out << "-------------------" << Qt::endl;

    QFile fileTelBin("nametel.bin");
    if (!fileTelBin.open(QIODevice::WriteOnly)) {
        cout << "Can't open nametel.bin" << Qt::endl;
        return 1;
    }
    QDataStream outTelBin(&fileTelBin);
    telBook.writeBin(outTelBin);
    fileTelBin.close();
    cout << "nametel.bin written" << Qt::endl;

    Solution<NameTel> telBook2;
    if (!fileTelBin.open(QIODevice::ReadOnly)) {
        cout << "Can't open nametel.bin for read" << Qt::endl;
        return 1;
    }
    QDataStream inTelBin(&fileTelBin);
    telBook2.readBin(inTelBin);
    fileTelBin.close();
    cout << "NameTel read from binary file" << Qt::endl;

    out << "Read NameTel from bin:" << Qt::endl;
    telBook2.writeText(out);
    out << "===================" << Qt::endl;

    // =========================== NameAddr
    QFile fileAddrTxt("nameaddr.txt");
    if (!fileAddrTxt.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Can't open nameaddr.txt" << Qt::endl;
        return 1;
    }
    cout << "nameaddr.txt opened" << Qt::endl;

    QTextStream inAddrTxt(&fileAddrTxt);
    Solution<NameAddr> addrBook;
    addrBook.readText(inAddrTxt);
    fileAddrTxt.close();

    out << "========== NameAddr ==========" << Qt::endl;
    out << "NameAddr from txt:" << Qt::endl;
    addrBook.writeText(out);
    out << "-------------------" << Qt::endl;

    addrBook.sortByName();
    out << "Sorted by name:" << Qt::endl;
    addrBook.writeText(out);
    out << "-------------------" << Qt::endl;

    std::pair<int, int> range2 = addrBook.findByName("Olga");
    if (range2.first != range2.second) {
        out << "Found NameAddr range:" << Qt::endl;
        for (int i = range2.first; i < range2.second; i++) {
            out << addrBook[i];
        }
    }
    else {
        out << "NameAddr not found" << Qt::endl;
    }
    out << "-------------------" << Qt::endl;

    addrBook.sortByTel1();
    out << "Sorted by tel1:" << Qt::endl;
    addrBook.writeText(out);
    out << "-------------------" << Qt::endl;

    QFile fileAddrBin("nameaddr.bin");
    if (!fileAddrBin.open(QIODevice::WriteOnly)) {
        cout << "Can't open nameaddr.bin" << Qt::endl;
        return 1;
    }
    QDataStream outAddrBin(&fileAddrBin);
    addrBook.writeBin(outAddrBin);
    fileAddrBin.close();
    cout << "nameaddr.bin written" << Qt::endl;

    Solution<NameAddr> addrBook2;
    if (!fileAddrBin.open(QIODevice::ReadOnly)) {
        cout << "Can't open nameaddr.bin for read" << Qt::endl;
        return 1;
    }
    QDataStream inAddrBin(&fileAddrBin);
    addrBook2.readBin(inAddrBin);
    fileAddrBin.close();
    cout << "NameAddr read from binary file" << Qt::endl;

    out << "Read NameAddr from bin:" << Qt::endl;
    addrBook2.writeText(out);
    out << "===================" << Qt::endl;

    outputFile.close();

    cout << "output.txt written" << Qt::endl;

    return 0;
}
