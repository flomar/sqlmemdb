#include "register_types.h"
#include "core/class_db.h"
#include "sqlmemdb.h"

void register_sqlmemdb_types() {
	ClassDB::register_class<SqlMemoryDatabase>();
}

void unregister_sqlmemdb_types() {

}
