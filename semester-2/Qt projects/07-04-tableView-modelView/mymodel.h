#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>
#include <QDataStream>
#include <QFile>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QVector>

struct WithTelephon {
    QString name;
    long long tel1;
    long long tel2;

    friend QTextStream& operator<<( QTextStream& out, const WithTelephon& rhs ) {
        out << rhs.name << Qt::endl << rhs.tel1 << Qt::endl << rhs.tel2 << Qt::endl << Qt::endl;
        return out;
    }

    friend QTextStream& operator>>( QTextStream& in, WithTelephon& rhs ) {
        in >> rhs.name >> rhs.tel1 >> rhs.tel2;
        return in;
    }

    operator bool() const {
        return !name.isEmpty();
    }

    friend QDataStream& operator<<( QDataStream& bout, const WithTelephon& rhs ) {
        bout << rhs.name << rhs.tel1 << rhs.tel2;
        return bout;
    }

    friend QDataStream& operator>>( QDataStream& bin, WithTelephon& rhs ) {
        bin >> rhs.name >> rhs.tel1 >> rhs.tel2;
        return bin;
    }
};

struct WithAddres {
    QString name;
    QString addr;
    long long tel1;

    friend QTextStream& operator<<( QTextStream& out, const WithAddres& rhs ) {
        out << rhs.name << Qt::endl << rhs.tel1 << Qt::endl << rhs.addr << Qt::endl << Qt::endl;
        return out;
    }

    friend QTextStream& operator>>( QTextStream& in, WithAddres& rhs ) {
        in >> rhs.name >> rhs.addr >> rhs.tel1;
        return in;
    }

    friend QDataStream& operator<<( QDataStream& bout, const WithAddres& rhs ) {
        bout << rhs.name << rhs.tel1 << rhs.addr;
        return bout;
    }

    friend QDataStream& operator>>( QDataStream& bin, WithAddres& rhs ) {
        bin >> rhs.name >> rhs.tel1 >> rhs.addr;
        return bin;
    }
};

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<WithTelephon> modelData;
    friend QTextStream& operator>>( QTextStream& in, MyModel& rhs );
public:
    explicit MyModel( QObject* parent = nullptr );
    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    void appendTelephon( const WithTelephon& value );
    int itemCount() const;
    bool saveToFile( const QString &filePath ) const;
};

class MyAddrModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<WithAddres> modelData;
    friend QTextStream& operator>>( QTextStream& in, MyAddrModel& rhs );
public:
    explicit MyAddrModel( QObject* parent = nullptr );
    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    int itemCount() const;
    bool saveToFile( const QString &filePath ) const;
};

QTextStream& operator>>( QTextStream& in, MyModel& rhs );
QTextStream& operator>>( QTextStream& in, MyAddrModel& rhs );

#endif // MYMODEL_H
