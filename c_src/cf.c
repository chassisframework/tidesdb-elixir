#include <erl_nif.h>
#include <tidesdb/db.h>
#include "cf.h"
#include "resources.h"
#include "atoms.h"
#include "errors.h"

ERL_NIF_TERM etdb_create_column_family(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  tidesdb_column_family_config_t cf_config = tidesdb_default_column_family_config();

  char name[4096];
  enif_get_string(env, argv[1], name, sizeof(name), ERL_NIF_UTF8);

  if (tidesdb_create_column_family(db, name, &cf_config) == TDB_SUCCESS)
    return ETDB_ATOM_OK;
  else
    return ETDB_ATOM_ERROR;
}

ERL_NIF_TERM etdb_get_column_family(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  char name[4096];
  enif_get_string(env, argv[1], name, sizeof(name), ERL_NIF_UTF8);

  tidesdb_column_family_t *cf = tidesdb_get_column_family(db, name);
  if (!cf)
    return etdb_tdb_error_tuple(env, TDB_ERR_NOT_FOUND);

  tidesdb_column_family_t **cf_resource = enif_alloc_resource(ETDB_CF_RESOURCE_TYPE, sizeof(tidesdb_column_family_t *));
  if (!cf_resource)
    return ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE;
  *cf_resource = cf;

  ERL_NIF_TERM cf_ref = enif_make_resource(env, cf_resource);
  enif_release_resource(cf_resource);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, cf_ref);
}

ERL_NIF_TERM etdb_list_column_families(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  char **names;
  int count;

  int result = tidesdb_list_column_families(db, &names, &count);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  ERL_NIF_TERM erl_names[count];

  for (int i = 0; i < count; i++) {
    erl_names[i] = enif_make_string(env, names[i], ERL_NIF_UTF8);
    tidesdb_free(names[i]);
  }

  tidesdb_free(names);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, enif_make_list_from_array(env, erl_names, count));
}

