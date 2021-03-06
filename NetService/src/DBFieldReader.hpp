#ifndef DB_FIELD_H_
#define DB_FIELD_H_

#include "DBDefine.h"
#include <sqlite3/sqlite3.h>

class DBFieldReader
{
    friend class DBCommand;

public:
    int fieldIndex() const
    {
        return _fieldIndex;
    }

    DBString fieldName() const
    {
#ifdef DB_UTF16
        return (const wchar_t*)sqlite3_column_name16(_stmt, _fieldIndex);
#else
        return sqlite3_column_name(_stmt, _fieldIndex);
#endif
        
    }

public:
    bool asBool() const
    {
        return sqlite3_column_int(_stmt, _fieldIndex) == 0 ? false : true;
    }

    int32 asInt32() const
    {
        return sqlite3_column_int(_stmt, _fieldIndex);;
    }

    int64 asInt64() const
    {
        int64 value = sqlite3_column_int64(_stmt, _fieldIndex);
        return value;
    }

    uint32 asUInt32() const
    {
        return (uint32)sqlite3_column_int64(_stmt, _fieldIndex);
    }

    uint64 asUInt64() const
    {
        return (uint64)sqlite3_column_int64(_stmt, _fieldIndex);
    }

    DBString asString() const
    {
#ifdef DB_UTF16
        return (const wchar_t*)sqlite3_column_text16(_stmt, _fieldIndex);
#else
        return (const char*)sqlite3_column_text(_stmt, _fieldIndex);
#endif
    }

private:
    DBFieldReader() 
        : _stmt(NULL), _fieldIndex(INVALID_FIELD_INDEX)
    {
    }

    void setReaderStmt(sqlite3_stmt* stmt, FieldKey_t indexN)
    {
        _stmt = stmt;
        _fieldIndex = indexN;
    }

    sqlite3_stmt* _stmt;
    FieldKey_t _fieldIndex;
};



#endif