// #include <QCoreApplication>
// #include <QString>
// #include <QTextStream>
// #include <QFile>
// #include <cstdlib>

// int compareStrings(const void* a, const void* b) {
//     const QString* s1 = static_cast<const QString*>(a);
//     const QString* s2 = static_cast<const QString*>(b);
//     return QString::compare(*s1, *s2);
// }

// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);

//     QTextStream out(stdout);

//     int n;

//     QFile file("/Users/alex/Documents/Qt projects/qt-lab-4/for_task_1.txt");
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         out << "file didnt open\n";
//         return 1;
//     }

//     QTextStream in(&file);

//     in >> n;
//     in.readLine();

//     QString* arr = new QString[n];

//     for (int i = 0; i < n; i++) {
//         arr[i] = in.readLine();
//     }

//     file.close();

//     qsort(arr, n, sizeof(QString), compareStrings);

//     out << "\nafter qsort ==> \n";
//     for (int i = 0; i < n; i++) {
//         out << arr[i] << "\n";
//     }

//     delete[] arr;
//     arr = nullptr;

//     return 0;
// }
