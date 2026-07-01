#ifndef BASEFANATS_H
#define BASEFANATS_H

#include "structfanat.h"
#include "abstractfanat.h"
#include <algorithm>
#include <utility>


class MB1: public ABS_Fanat{
private:
    int count;
    QVector<Fanat1> M;
public:
    MB1(): count(0){
        M.resize(0);
    };
    MB1(const MB1& rhs) : count(rhs.count), M(rhs.M) {}
    MB1(MB1&& rhs) noexcept:count(std::move(rhs.count)), M(std::move(rhs.M)){};
    ~MB1() override = default;
    MB1& operator=(const MB1& rhs){
        if(this != &rhs){
            count = rhs.count;
            M = rhs.M;
        }
        return *this;
    };
    MB1& operator=(MB1&& rhs){
        count = std::move(rhs.count);
        M = std::move(rhs.M);
        return *this;
    };

    void GetFanats(QTextStream& in)override{
        count = in.readLine().toInt();
        M.resize(count);
        for (Fanat1& el : M) {
            in >> el;
        }
    };
    void PutFanats(QTextStream& out)override{
        for (const Fanat1& el : M) {
            out << el << "\n";
        }
    };

    void SortUp()override{
        std::sort(M.begin(), M.end(), [](const Fanat1& a, const Fanat1& b) {
            return a.Name < b.Name;
        });
    }

    void SortDown()override{
        std::sort(M.begin(), M.end(), [](const Fanat1& a, const Fanat1& b) {
            return a.Name > b.Name;
        });
    }
    QVector<int> Find(const QString& name) const
    {
        QVector<int> range;

        auto lower = std::lower_bound(M.begin(), M.end(), name, [](const Fanat1& fanat, const QString& value) {
            return fanat.Name < value;
        });
        auto upper = std::upper_bound(lower, M.end(), name, [](const QString& value, const Fanat1& fanat) {
            return value < fanat.Name;
        });

        if (lower == upper)
            return range;

        range.push_back(lower - M.begin());
        range.push_back(upper - M.begin());
        return range;
    }

    int GetCount() const{
        return count;
    };

    Fanat1 GetFanat(int i) const{
        return M[i];
    };

};


#endif // BASEFANATS_H
