#ifndef PLENTY_H
#define PLENTY_H
#include "QVector"
#include "QDebug"
#include "QString"
#include "QFile"
#include "QTextStream"

class Abstract_Plenty
{
public:
    virtual ~Abstract_Plenty(){};
    virtual void add(Abstract_Plenty&, Abstract_Plenty&){};
    virtual void mult(Abstract_Plenty&, Abstract_Plenty&){};
    virtual void sim_sub(Abstract_Plenty&, Abstract_Plenty&){};
    virtual void print(){};
    virtual QString toString() const { return ""; };
    virtual bool readFromFile(const QString&){ return false; };
    virtual bool writeToFile(const QString&) const { return false; };
};

class MyPlenty_int: public Abstract_Plenty
{
private:
    QVector<int> data;
public:
    MyPlenty_int(): data() {};
    MyPlenty_int(QVector<int> data): data(data){};
    MyPlenty_int(const MyPlenty_int& rhs): data(rhs.data) {};
    MyPlenty_int(MyPlenty_int&& rhs): data(std::move(rhs.data)) {};

    MyPlenty_int& operator=(const MyPlenty_int& rhs){
        data.clear();
        data = rhs.data;
        return *this;
    };
    MyPlenty_int& operator=(MyPlenty_int&& rhs){
        data.clear();
        data = std::move(rhs.data);
        return *this;
    };



    void add(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_int* second = static_cast<MyPlenty_int*>(&lhs);
        MyPlenty_int* first = static_cast<MyPlenty_int*>(&rhs);
        data = (second->data);
        for(int& el1 : first->data){
            bool contain = 0;
            for(int& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
        //return temp;
    };
    void mult(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_int* second = static_cast<MyPlenty_int*>(&lhs);
        MyPlenty_int* first = static_cast<MyPlenty_int*>(&rhs);
        data.clear();
        for(int& el1 : first->data){
            bool contain = 0;
            for(int& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(contain) data.push_back(el1);
        }
        //return temp;
    };
    void sim_sub(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_int* second = static_cast<MyPlenty_int*>(&lhs);
        MyPlenty_int* first = static_cast<MyPlenty_int*>(&rhs);
        data.clear();
        for(int& el1 : second->data){
            bool contain = 0;
            for(int& el2 : first->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
        for(int& el1 : first->data){
            bool contain = 0;
            for(int& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
        //return temp;
    };

    void print() override
    {
        qDebug() << " -- " << data << "\n";
    };

    QString toString() const override
    {
        QString result;
        for(int i = 0; i < data.size(); ++i){
            result += QString::number(data[i]);
            if(i + 1 < data.size()){
                result += " ";
            }
        }
        return result;
    };

    bool readFromFile(const QString& fileName) override
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "Cannot open file for reading:" << fileName;
            return false;
        }

        data.clear();
        QTextStream in(&file);
        int value;
        while(!in.atEnd()){
            in >> value;
            if(in.status() == QTextStream::Ok){
                data.push_back(value);
            }
        }

        return true;
    };

    bool writeToFile(const QString& fileName) const override
    {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Cannot open file for writing:" << fileName;
            return false;
        }

        QTextStream out(&file);
        for(int i = 0; i < data.size(); ++i){
            out << data[i];
            if(i + 1 < data.size()){
                out << " ";
            }
        }
        out << "\n";

        return true;
    };
};

// ------------------------------------------------------------------------------




class MyPlenty_sring: public Abstract_Plenty
{
private:
    QVector<QString> data;
public:
    MyPlenty_sring(): data() {};
    MyPlenty_sring(QVector<QString> data): data(data){};
    MyPlenty_sring(const MyPlenty_sring& rhs): data(rhs.data) {};
    MyPlenty_sring(MyPlenty_sring&& rhs): data(std::move(rhs.data)) {};

    MyPlenty_sring& operator=(const MyPlenty_sring& rhs){
        data.clear();
        data = rhs.data;
        return *this;
    };
    MyPlenty_sring& operator=(MyPlenty_sring&& rhs){
        data.clear();
        data = std::move(rhs.data);
        return *this;
    };

    void add(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_sring* second = static_cast<MyPlenty_sring*>(&lhs);
        MyPlenty_sring* first = static_cast<MyPlenty_sring*>(&rhs);
        data = (second->data);
        for(QString& el1 : first->data){
            bool contain = 0;
            for(QString& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
    };
    void mult(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_sring* second = static_cast<MyPlenty_sring*>(&lhs);
        MyPlenty_sring* first = static_cast<MyPlenty_sring*>(&rhs);
        data.clear();
        for(QString& el1 : first->data){
            bool contain = 0;
            for(QString& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(contain) data.push_back(el1);
        }
    };
    void sim_sub(Abstract_Plenty& lhs, Abstract_Plenty& rhs) override {
        MyPlenty_sring* second = static_cast<MyPlenty_sring*>(&lhs);
        MyPlenty_sring* first = static_cast<MyPlenty_sring*>(&rhs);
        data.clear();
        for(QString& el1 : second->data){
            bool contain = 0;
            for(QString& el2 : first->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
        for(QString& el1 : first->data){
            bool contain = 0;
            for(QString& el2 : second->data){
                if(el1 == el2){
                    contain = 1;
                }
            }
            if(!contain) data.push_back(el1);
        }
    };

    void print() override
    {
        qDebug() << " -- " << data << "\n";
    };

    QString toString() const override
    {
        QString result;
        for(int i = 0; i < data.size(); ++i){
            result += data[i];
            if(i + 1 < data.size()){
                result += " ";
            }
        }
        return result;
    };

    bool readFromFile(const QString& fileName) override
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "Cannot open file for reading:" << fileName;
            return false;
        }

        data.clear();
        QTextStream in(&file);
        QString value;
        while(!in.atEnd()){
            in >> value;
            if(in.status() == QTextStream::Ok && !value.isEmpty()){
                data.push_back(value);
            }
        }

        return true;
    };

    bool writeToFile(const QString& fileName) const override
    {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Cannot open file for writing:" << fileName;
            return false;
        }

        QTextStream out(&file);
        for(int i = 0; i < data.size(); ++i){
            out << data[i];
            if(i + 1 < data.size()){
                out << " ";
            }
        }
        out << "\n";

        return true;
    };
};




#endif // PLENTY_H
