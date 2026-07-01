#ifndef STRUCTFANAT_H
#define STRUCTFANAT_H


#include <QTextStream>
#include <QString>
#include <QVector>

using namespace std;

struct Fanat1{
    QString Name;
    QString Nationality;
    int Year;
    int N;

    QString GetName() const { return Name; }
    void SetName(const QString& x) { Name = x; }

    friend QTextStream& operator>>(QTextStream& in, Fanat1& f) {
        f.Name = in.readLine();
        f.Nationality = in.readLine();
        in >> f.Year;
        in >> f.N;
        in.readLine();
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Fanat1& f) {
        out << f.Name << '\t' << f.Nationality << '\t' << f.Year << '\t' << f.N;
        return out;
    }
};
struct Fanat2{
    QString Name;
    int Code, N;

    QString GetName() const { return Name; }
    void SetName(const QString& x) { Name = x; }

    friend QTextStream& operator>>(QTextStream& in, Fanat2& f) {
        f.Name = in.readLine();
        in >> f.Code;
        in >> f.N;
        in.readLine();
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Fanat2& f) {
        out << f.Name << '\t' << f.Code << '\t' << f.N;
        return out;
    }
};
struct Fanat3{
    QString Nationality;
    QString Name;
    int Year, Code, N;

    QString GetName() const { return Name; }
    void SetName(const QString& x) { Name = x; }

    friend QTextStream& operator>>(QTextStream& in, Fanat3& f) {
        f.Nationality = in.readLine();
        f.Name = in.readLine();
        in >> f.Year;
        in >> f.Code;
        in >> f.N;
        in.readLine();
        return in;
    }
    friend QTextStream& operator<<(QTextStream& out, const Fanat3& f) {
        out << f.Nationality << '\t' << f.Name << '\t' << f.Year << '\t' << f.N << '\t' << f.Code;
        return out;
    }
};

#endif // STRUCTFANAT_H
