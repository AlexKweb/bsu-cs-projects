// #include <QCoreApplication>
// #include <QString>
// #include <QTextStream>
// #include <QFile>

// QString func(QString &s, QString &word, QString &wordInsert){
//     int pos = 0;

//     while ((pos = s.indexOf(word, pos)) != -1)
//     {
//         pos = pos + word.length();
//         s.insert(pos, " " + wordInsert);
//         pos += wordInsert.length() + 1;
//     }

//     return s;
// }

// int main(int argc, char *argv[]){
//     QCoreApplication a(argc, argv);

//     QTextStream out(stdout);

//     QFile file("/Users/alex/Documents/Qt projects/qt-lab-4/for_task_3.txt");
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

//     file.close();

//     out << "Final out:" << func(s, word, wordInsert) << "\n";

//     return 0;
// }
