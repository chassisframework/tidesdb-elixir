#include <erl_nif.h>
#include <tidesdb/db.h>
#include "db.h"
#include "resources.h"
#include "atoms.h"
#include "errors.h"

// should we make the reference object close the db when the last process that holds a ref dies?
ERL_NIF_TERM etdb_open(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  char db_path[4096];
  if (enif_get_string(env, argv[0], db_path, sizeof(db_path), ERL_NIF_UTF8) <= 0)
    return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, ETDB_ATOM_ENIF_GET_STRING);

  tidesdb_config_t config = tidesdb_default_config();
  config.db_path = db_path;
  config.log_level = TDB_LOG_WARN;

  tidesdb_t *db;
  int result = tidesdb_open(&config, &db);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  tidesdb_t **db_resource = enif_alloc_resource(ETDB_DB_RESOURCE_TYPE, sizeof(tidesdb_t *));
  if (!db_resource)
    return ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE;
  *db_resource = db;

  ERL_NIF_TERM db_ref = enif_make_resource(env, db_resource);
  enif_release_resource(db_resource);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, db_ref);
}

ERL_NIF_TERM etdb_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  int result = tidesdb_close(db);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  return ETDB_ATOM_OK;
}
