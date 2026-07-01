// #include <QCoreApplication>
// #include <QString>
// #include <QTextStream>
// #include <QFile>

// QString func(QString &s, QString &word){
//     while (s.indexOf(word) != -1)
//     {
//         s.remove(s.indexOf(word), word.length() + 1);

//     }

//     for (QChar c : {' ', ',', '.', '!', '?', ';', ':'}) {
//         QString pattern = " ";
//         pattern += c;
//         if (s.indexOf(pattern) != -1) {
//             s.remove(s.indexOf(pattern), 1);
//         }
//     }

//     return s;
// }

// int main(int argc, char *argv[]){
//     QCoreApplication a(argc, argv);

//     QTextStream out(stdout);

//     QFile file("/Users/alex/Documents/Qt projects/qt-lab-4/for_task_2.txt");
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         out << "file didnt open\n";
//         return 1;
//     }

//     QTextStream in(&file);

//     QString s = in.readLine();
//     QString word = in.readLine();

//     file.close();

//     out << "Final out: " << func(s, word) << "\n\n";

//     return 0;
// }
