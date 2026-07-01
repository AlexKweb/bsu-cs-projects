#include <QCoreApplication>
#include <QString>
#include <QTextStream>
#include <QFile>

void func(QString &s, int &old_num, int &new_num, QString &s2){
    QString old_str = QString::number(old_num);
    QString new_str = QString::number(new_num);

    if (s.indexOf(old_str) != -1)
    {
        int pos = s.indexOf(old_str);
        s.remove(pos, old_str.length());
        s.insert(pos, new_str);
    }

    s2 = s;
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);

    QFile file("/Users/alex/Documents/Qt projects/qt-lab-4/for_task_5.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        out << "file didnt open\n";
        return 1;
    }

    QTextStream in(&file);

    QString s;
    s = in.readLine();

    int old_num;
    in >> old_num;

    int new_num;
    in >> new_num;

    QString s2;
    func(s, old_num, new_num, s2);

    file.close();

    out << "Final out:" << s2 << "\n";

    return 0;
}
