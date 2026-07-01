#include <QCoreApplication>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QString>
#include <algorithm>

struct Student {
    QString name;
    QString surname;
    int course;
    int group;
    double avg;

    struct CompareSurname {
        bool operator()(const Student& a, const Student& b) const {
            return a.surname < b.surname;
        }
    };

    struct CompareCourseGroupSurname {
        bool operator()(const Student& a, const Student& b) const {
            if (a.course != b.course) {
                return a.course < b.course;
            }
            if (a.group != b.group) {
                return a.group < b.group;
            }
            return a.surname < b.surname;
        }
    };
};

QTextStream& operator>>(QTextStream& in, Student& rhs) {
    in >> rhs.name >> rhs.surname >> rhs.course >> rhs.group >> rhs.avg;
    return in;
}

QTextStream& operator<<(QTextStream& out, const Student& rhs) {
    out << rhs.name << ' '
        << rhs.surname << ' '
        << rhs.course << ' '
        << rhs.group << ' '
        << rhs.avg << Qt::endl;
    return out;
}

QDataStream& operator<<(QDataStream& out, const Student& rhs) {
    out << rhs.name << rhs.surname << rhs.course << rhs.group << rhs.avg;
    return out;
}

QDataStream& operator>>(QDataStream& in, Student& rhs) {
    in >> rhs.name >> rhs.surname >> rhs.course >> rhs.group >> rhs.avg;
    return in;
}

template <typename T>
class Solution {
private:
    QVector<T> structures;

public:
    Solution() = default;

    friend QTextStream& operator>>(QTextStream& in, Solution<T>& rhs) {
        rhs.structures.clear();

        while (!in.atEnd()) {
            T temp;
            in >> temp;

            if (in.status() != QTextStream::Ok) {
                break;
            }

            rhs.structures.push_back(temp);
        }

        return in;
    }

    friend QTextStream& operator<<(QTextStream& out, const Solution<T>& rhs) {
        int size = rhs.structures.size();
        for (int i = 0; i < size; i++) {
            out << rhs.structures[i];
        }
        return out;
    }

    friend QDataStream& operator<<(QDataStream& out, const Solution<T>& rhs) {
        int size = rhs.structures.size();
        out << size;

        for (int i = 0; i < size; i++) {
            out << rhs.structures[i];
        }

        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, Solution<T>& rhs) {
        rhs.structures.clear();

        int size;
        in >> size;

        for (int i = 0; i < size; i++) {
            T temp;
            in >> temp;
            rhs.structures.push_back(temp);
        }

        return in;
    }

    template <typename Comparator>
    void sort_data(Comparator comp) {
        std::sort(structures.begin(), structures.end(), comp);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);

    QFile inTxt("students.txt");
    if (!inTxt.open(QFileDevice::ReadOnly | QFileDevice::Text)) {
        out << "Can't open students.txt" << Qt::endl;
        return 1;
    }

    QFile outTxt("res.txt");
    if (!outTxt.open(QFileDevice::WriteOnly | QFileDevice::Text)) {
        out << "Can't open res.txt" << Qt::endl;
        return 1;
    }

    QFile binFile("result.bin");
    if (!binFile.open(QFileDevice::ReadWrite)) {
        out << "Can't open result.bin" << Qt::endl;
        return 1;
    }

    QTextStream inTxtTest(&inTxt);
    QTextStream outTxtTest(&outTxt);
    QDataStream outBinTest(&binFile);
    QDataStream inBinTest(&binFile);

    Solution<Student> iotest;

    inTxtTest >> iotest;
    out << "Read from txt:" << Qt::endl;
    out << iotest;
    out << "-------------------------" << Qt::endl;

    iotest.sort_data(Student::CompareSurname());
    out << "Sorted by surname:" << Qt::endl;
    out << iotest;
    out << "-------------------------" << Qt::endl;

    iotest.sort_data(Student::CompareCourseGroupSurname());
    out << "Sorted by course, group, surname:" << Qt::endl;
    out << iotest;
    out << "-------------------------" << Qt::endl;

    outBinTest << iotest;
    out << "Wrote in bin file result.bin!" << Qt::endl;
    out << "-------------------------" << Qt::endl;

    outTxtTest << iotest;
    out << "Wrote in txt file res.txt!" << Qt::endl;
    out << "-------------------------" << Qt::endl;

    binFile.seek(0);

    Solution<Student> fromBin;
    inBinTest >> fromBin;
    out << "Read from bin file result.bin!" << Qt::endl;
    out << "-------------------------" << Qt::endl;
    out << fromBin;

    inTxt.close();
    outTxt.close();
    binFile.close();

    return 0;
}
