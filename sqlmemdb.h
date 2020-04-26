#ifndef SQLMEMDB_H
#define SQLMEMDB_H

#include "core/os/os.h"
#include "core/reference.h"

#include "sqlite/sqlite3.h"

#include <string>

class SqlMemoryDatabase : public Reference {
	GDCLASS(SqlMemoryDatabase, Reference);
public:
	SqlMemoryDatabase();
	~SqlMemoryDatabase();
public:
	bool open();
	void close();
public:
	bool query(const String &_query);
public:
	const Array &query_result() { return m_query_result; }
	const String &query_error() { return m_query_error; }
protected:
	static int query_callback(void *_that, int _column_count, char **_column_names, char **_result_column_names);
	static void update_callback(void *_that, int _operation, const char *_database, const char *_table, sqlite3_int64 _row);
protected:
	String create_info_message(const String &_message);
	String create_error_message(const String &_message);
protected:
	void print_info_message(const String &_message);
	void print_error_message(const String &_message);
protected:
	static void _bind_methods();
private:
	Array m_query_result;
	String m_query_error;
private:
	sqlite3 *m_database;
};

#endif
