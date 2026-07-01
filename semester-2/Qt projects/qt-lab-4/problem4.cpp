// #include <QCoreApplication>
// #include <QString>
// #include <QTextStream>
// #include <QFile>

// void func(QString &s, QString &word, QString &wordInsert, QString &s2){
//     if (s.indexOf(word) != -1)
//     {
//         int pos = s.indexOf(word);
//         s.remove(pos, word.length());
//         s.insert(pos, wordInsert);
//     }
//     s2 = s;
// }

// int main(int argc, char *argv[]){
//     QCoreApplication a(argc, argv);

//     QTextStream out(stdout);

//     QFile file("/Users/alex/Documents/Qt projects/qt-lab-4/for_task_4.txt");
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         out << "file didnt open\n";
//         return 1;
//     }

//     QTextStream in(&file);

//     QString s;
//     s = in.readLine();

//     QString word;
//     word = in.readLine();

//     QString wordInsert;
//     wordInsert = in.readLine();

//     QString s2;
//     func(s, word, wordInsert, s2);

//     file.close();

//     out << "Final out:" << s2 << "\n";

//     return 0;
// }
