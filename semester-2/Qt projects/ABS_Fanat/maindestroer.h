#ifndef MAINDESTROER_H
#define MAINDESTROER_H

#include "structfanat.h"
#include "abstractfanat.h"
#include "basefanats.h"
#include "fanats2024.h"
#include <algorithm>
#include <utility>

class MB3: public ABS_Fanat{
private:
    int count;
    QVector<Fanat3> M;
public:
    MB3(): count(0){
        M.resize(0);
    };
    MB3(const MB3& rhs) : count(rhs.count), M(rhs.M) {}
    MB3(MB3&& rhs) noexcept:count(std::move(rhs.count)), M(std::move(rhs.M)){};
    ~MB3() override = default;

    MB3& operator=(const MB3& rhs){
        if(this != &rhs){
            count = rhs.count;
            M = rhs.M;
        }
        return *this;
    };

    MB3& operator=(MB3&& rhs){
        count = std::move(rhs.count);
        M = std::move(rhs.M);
        return *this;
    };

    void GetFanats(QTextStream& in)override{
        in >> count;
        in.readLine();
        M.resize(count);
        for (Fanat3& el : M) {
            in >> el;
        }
    };

    void PutFanats(QTextStream& out)override{
        for (const Fanat3& el : M) {
            out << el << "\n";
        }
    };

    void SortUp()override{
        std::sort(M.begin(), M.end(), [](const Fanat3& a, const Fanat3& b) {
            return a.Name < b.Name;
        });
    }

    void SortDown()override{
        std::sort(M.begin(), M.end(), [](const Fanat3& a, const Fanat3& b) {
            return a.Name > b.Name;
        });
    }

    QVector<int> Find(const QString& name) const
    {
        QVector<int> range;

        auto lower = std::lower_bound(M.begin(), M.end(), name, [](const Fanat3& fanat, const QString& value) {
            return fanat.Name < value;
        });
        auto upper = std::upper_bound(lower, M.end(), name, [](const QString& value, const Fanat3& fanat) {
            return value < fanat.Name;
        });

        if (lower == upper)
            return range;

        range.push_back(lower - M.begin());
        range.push_back(upper - M.begin());
        return range;
    }

    void Common(const MB1& first, const MB2& second)
    {
        M.clear();
        MB2 secondCopy = second;
        secondCopy.SortUp();
        for (int i = 0; i < first.GetCount(); i++)
        {
            Fanat1 f1 = first.GetFanat(i);

            QVector<int> range = secondCopy.Find(f1.Name);

            if (range.size() == 2)
            {
                for (int index = range[0]; index < range[1]; index++)
                {
                    Fanat2 f2 = secondCopy.GetFanat(index);
                    Fanat3 f3;

                    f3.Nationality = f1.Nationality;
                    f3.Name = f1.Name;
                    f3.Year = f1.Year;
                    f3.Code = f2.Code;
                    f3.N = f1.N + f2.N;

                    M.push_back(f3);
                }

            }
        }

        count = M.size();
    }

    int GetCount() const{
        return count;
    };

    Fanat3 GetFanat(int i) const{
        return M[i];
    };
};


#endif // MAINDESTROER_H
