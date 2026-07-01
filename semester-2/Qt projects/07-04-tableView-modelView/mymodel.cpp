#include "mymodel.h"

MyModel::MyModel( QObject *parent )
    : QAbstractTableModel( parent )
{
}

int MyModel::rowCount( const QModelIndex &parent ) const {
    if( parent.isValid() ) return 0;
    return modelData.size();
}

int MyModel::columnCount( const QModelIndex &parent ) const {
    if( parent.isValid() ) return 0;
    return 3;
}

QVariant MyModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole )
        return QVariant();
    if( orientation == Qt::Horizontal )
    {
        switch( section )
        {
        case 0: return QVariant( tr( "Name" ) );
        case 1: return QVariant( tr( "Telephon1" ) );
        case 2: return QVariant( tr( "Telephon2" ) );
        default: return QVariant();
        }
    }
    else
        return QVariant( tr( "%1" ).arg( section + 1 ) );
}

QVariant MyModel::data( const QModelIndex &index, int role ) const
{
    if( ( role != Qt::DisplayRole && role != Qt::EditRole ) || !index.isValid() || index.row() >= modelData.size() )
        return QVariant();
    switch( index.column() )
    {
        case 0: return modelData[index.row()].name;
        case 1: return modelData[index.row()].tel1;
        case 2: return modelData[index.row()].tel2;
    default: return QVariant();
    }
}

Qt::ItemFlags MyModel::flags( const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MyModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( role != Qt::EditRole || !index.isValid() || index.row() >= modelData.size() ) {
        return false;
    }

    bool ok = false;
    switch( index.column() )
    {
        case 0:
            modelData[index.row()].name = value.toString();
            break;
        case 1:
            modelData[index.row()].tel1 = value.toLongLong(&ok);
            if( !ok ) return false;
            break;
        case 2:
            modelData[index.row()].tel2 = value.toLongLong(&ok);
            if( !ok ) return false;
            break;
        default:
            return false;
    }

    emit dataChanged(index, index);
    return true;
}

void MyModel::appendTelephon( const WithTelephon& value )
{
    const int row = modelData.size();
    beginInsertRows( QModelIndex(), row, row );
    modelData.append( value );
    endInsertRows();
}

int MyModel::itemCount() const
{
    return modelData.size();
}

bool MyModel::saveToFile( const QString &filePath ) const
{
    QFile file(filePath);
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        return false;
    }

    QTextStream out(&file);
    for( const WithTelephon &value : modelData ) {
        out << value.name << ' ' << value.tel1 << ' ' << value.tel2 << Qt::endl;
    }

    return true;
}

QTextStream& operator>>( QTextStream& in, MyModel& rhs )
{
    rhs.beginResetModel();
    rhs.modelData.clear();

    while( !in.atEnd() ) {
        WithTelephon value;
        in >> value;
        if( value ) {
            rhs.modelData.append( value );
        }
    }
    rhs.endResetModel();

    return in;
}

MyAddrModel::MyAddrModel( QObject *parent )
    : QAbstractTableModel( parent )
{
}

int MyAddrModel::rowCount( const QModelIndex &parent ) const {
    if( parent.isValid() ) return 0;
    return modelData.size();
}

int MyAddrModel::columnCount( const QModelIndex &parent ) const {
    if( parent.isValid() ) return 0;
    return 3;
}

QVariant MyAddrModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole )
        return QVariant();
    if( orientation == Qt::Horizontal )
    {
        switch( section )
        {
        case 0: return QVariant( tr( "Name" ) );
        case 1: return QVariant( tr( "Address" ) );
        case 2: return QVariant( tr( "Telephon1" ) );
        default: return QVariant();
        }
    }
    else
        return QVariant( tr( "%1" ).arg( section + 1 ) );
}

QVariant MyAddrModel::data( const QModelIndex &index, int role ) const
{
    if( ( role != Qt::DisplayRole && role != Qt::EditRole ) || !index.isValid() || index.row() >= modelData.size() )
        return QVariant();
    switch( index.column() )
    {
        case 0: return modelData[index.row()].name;
        case 1: return modelData[index.row()].addr;
        case 2: return modelData[index.row()].tel1;
        default: return QVariant();
    }
}

Qt::ItemFlags MyAddrModel::flags( const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MyAddrModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( role != Qt::EditRole || !index.isValid() || index.row() >= modelData.size() ) {
        return false;
    }

    bool ok = false;
    switch( index.column() )
    {
        case 0:
            modelData[index.row()].name = value.toString();
            break;
        case 1:
            modelData[index.row()].addr = value.toString();
            break;
        case 2:
            modelData[index.row()].tel1 = value.toLongLong(&ok);
            if( !ok ) return false;
            break;
        default:
            return false;
    }

    emit dataChanged(index, index);
    return true;
}

int MyAddrModel::itemCount() const
{
    return modelData.size();
}

bool MyAddrModel::saveToFile( const QString &filePath ) const
{
    QFile file(filePath);
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        return false;
    }

    QTextStream out(&file);
    for( const WithAddres &value : modelData ) {
        out << value.name << ' ' << value.addr << ' ' << value.tel1 << Qt::endl;
    }

    return true;
}

QTextStream& operator>>( QTextStream& in, MyAddrModel& rhs )
{
    rhs.beginResetModel();
    rhs.modelData.clear();

    while( !in.atEnd() ) {
        WithAddres value;
        in >> value;
        if( !value.name.isEmpty() ) {
            rhs.modelData.append( value );
        }
    }
    rhs.endResetModel();

    return in;
}
