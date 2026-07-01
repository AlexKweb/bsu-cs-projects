#ifndef FANATS2_24_H
#define FANATS2_24_H

#include "structfanat.h"
#include "abstractfanat.h"
#include <algorithm>
#include <utility>


class MB2: public ABS_Fanat{
private:
    int count;
    QVector<Fanat2> M;
public:
    MB2(): count(0){
        M.resize(0);
    };
    MB2(const MB2& rhs) : count(rhs.count), M(rhs.M) {}
    MB2(MB2&& rhs) noexcept:count(std::move(rhs.count)), M(std::move(rhs.M)){};
    ~MB2() override = default;

    MB2& operator=(const MB2& rhs){
        if(this != &rhs){
            count = rhs.count;
            M = rhs.M;
        }
        return *this;
    };

    MB2& operator=(MB2&& rhs){
        count = std::move(rhs.count);
        M = std::move(rhs.M);
        return *this;
    };

    void GetFanats(QTextStream& in)override{
        count = in.readLine().toInt();
        M.resize(count);
        for (Fanat2& el : M) {
            in >> el;
        }
    };

    void PutFanats(QTextStream& out)override{
        for (const Fanat2& el : M) {
            out << el << "\n";
        }
    };

    void SortUp()override{
        std::sort(M.begin(), M.end(), [](const Fanat2& a, const Fanat2& b) {
            return a.Name < b.Name;
        });
    }

    void SortDown()override{
        std::sort(M.begin(), M.end(), [](const Fanat2& a, const Fanat2& b) {
            return a.Name > b.Name;
        });
    }

    QVector<int> Find(const QString& name) const
    {
        QVector<int> range;

        auto lower = std::lower_bound(M.begin(), M.end(), name, [](const Fanat2& fanat, const QString& value) {
            return fanat.Name < value;
        });
        auto upper = std::upper_bound(lower, M.end(), name, [](const QString& value, const Fanat2& fanat) {
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

    Fanat2 GetFanat(int i) const{
        return M[i];
    };
};


#endif // FANATS2_24_H
