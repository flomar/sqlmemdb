#include "sqlmemdb.h"

SqlMemoryDatabase::SqlMemoryDatabase() :
	m_database(nullptr) {
	MethodInfo mi;
	mi.name = "signal_updated";
	ClassDB::add_signal("SqlMemoryDatabase", mi);
}

SqlMemoryDatabase::~SqlMemoryDatabase() {
	close();
}

bool SqlMemoryDatabase::open() {
	char *error = 0;
	if(m_database) {
		close();
	}
	if(sqlite3_open(":memory:", &m_database) != SQLITE_OK) {
		print_error_message(String(sqlite3_errmsg(m_database)));
		return false;
	}
	if(sqlite3_exec(m_database, "PRAGMA foreign_keys=on;", nullptr, nullptr, &error) != SQLITE_OK) {
		print_error_message(String(error));
		sqlite3_free(error);
		return false;
	}
	sqlite3_update_hook(m_database, &SqlMemoryDatabase::update_callback, (void*)(this));
	return true;
}

void SqlMemoryDatabase::close() {
	if(!m_database) {
		return;
	}
	if(sqlite3_close_v2(m_database) != SQLITE_OK) {
		print_error_message("Cannot close database!");
		return;
	}
	m_database = nullptr;
}

bool SqlMemoryDatabase::query(const String &_query) {
	char *error = 0;
	m_query_result.clear();
	const CharString query = _query.utf8();
	if(sqlite3_exec(m_database, query.get_data(), &SqlMemoryDatabase::query_callback, (void*)(this), &error) != SQLITE_OK) {
		print_error_message(error);
		m_query_error = create_error_message(error);
		sqlite3_free(error);
		return false;
	}
	return true;
}

int SqlMemoryDatabase::query_callback(void *_that, int _column_count, char **_column_names, char **_result_column_names) {
	SqlMemoryDatabase *that = static_cast<SqlMemoryDatabase*>(_that);
	sqlite3_stmt *stmt = sqlite3_next_stmt(that->m_database, nullptr);
	Dictionary column_dictionary;
	Variant column_value;
	for(int column_index = 0; column_index < _column_count; column_index++) {
		switch(sqlite3_column_type(stmt, column_index)) {
			case SQLITE_INTEGER:
				column_value = Variant((int)(sqlite3_column_int64(stmt, column_index)));
				break;
			case SQLITE_FLOAT:
				column_value = Variant(sqlite3_column_double(stmt, column_index));
				break;
			case SQLITE_TEXT:
				column_value = Variant((char*)(sqlite3_column_text(stmt, column_index)));
				break;
			default:
				column_value = Variant((char*)(sqlite3_column_text(stmt, column_index)));
				break;
		}
		column_dictionary[String(_result_column_names[column_index])] = column_value;
	}
	that->m_query_result.append(column_dictionary);
	return SQLITE_OK;
}

void SqlMemoryDatabase::update_callback(void *_that, int _operation, const char *_database, const char *_table, sqlite3_int64 _row) {
	SqlMemoryDatabase *that = static_cast<SqlMemoryDatabase*>(_that);
	that->emit_signal("signal_updated", nullptr, 0);
}

void SqlMemoryDatabase::_bind_methods() {
	ClassDB::bind_method("open", &SqlMemoryDatabase::open);
	ClassDB::bind_method("close", &SqlMemoryDatabase::close);
	ClassDB::bind_method("query", &SqlMemoryDatabase::query);
	ClassDB::bind_method("query_result", &SqlMemoryDatabase::query_result);
	ClassDB::bind_method("query_error", &SqlMemoryDatabase::query_error);
}

String SqlMemoryDatabase::create_info_message(const String &_message) {
	return String("[SqlMemoryDatabase|INFO] ") + _message;
}

String SqlMemoryDatabase::create_error_message(const String &_message) {
	return String("[SqlMemoryDatabase|ERROR] ") + _message;
}

void SqlMemoryDatabase::print_info_message(const String &_message) {
	const String message = create_info_message(_message + "\n");
	OS::get_singleton()->print(message.utf8());
}

void SqlMemoryDatabase::print_error_message(const String &_message) {
	const String message = create_error_message(_message + "\n");
	OS::get_singleton()->print(message.utf8());
}
