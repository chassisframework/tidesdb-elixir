#include <erl_nif.h>
#include <tidesdb/db.h>
#include "txn.h"
#include "atoms.h"
#include "resources.h"
#include "errors.h"

ERL_NIF_TERM etdb_txn_begin(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  tidesdb_txn_t *txn = NULL;
  int result = tidesdb_txn_begin(db, &txn);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  tidesdb_txn_t **txn_resource = enif_alloc_resource(ETDB_TXN_RESOURCE_TYPE, sizeof(tidesdb_txn_t *));
  if (!txn_resource)
    return ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE;
  *txn_resource = txn;

  ERL_NIF_TERM txn_ref = enif_make_resource(env, txn_resource);
  enif_release_resource(txn_resource);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, txn_ref);
}

ERL_NIF_TERM etdb_txn_put(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  tidesdb_column_family_t **cf_resource;
  if (!enif_get_resource(env, argv[1], ETDB_CF_RESOURCE_TYPE, (void *)&cf_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_column_family_t *cf = *cf_resource;

  ErlNifBinary key, value;

  if (!enif_inspect_binary(env, argv[2], &key))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  if (!enif_inspect_binary(env, argv[3], &value))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  // TODO: support TTL
  int result = tidesdb_txn_put(txn, cf, key.data, key.size, value.data, value.size, -1);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_txn_get(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  tidesdb_column_family_t **cf_resource;
  if (!enif_get_resource(env, argv[1], ETDB_CF_RESOURCE_TYPE, (void *)&cf_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_column_family_t *cf = *cf_resource;

  ErlNifBinary key;
  if (!enif_inspect_binary(env, argv[2], &key))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  ErlNifBinary value;
  int result = tidesdb_txn_get(txn, cf, key.data, key.size, &value.data, &value.size);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, enif_make_binary(env, &value));
}

ERL_NIF_TERM etdb_txn_delete(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  tidesdb_column_family_t **cf_resource;
  if (!enif_get_resource(env, argv[1], ETDB_CF_RESOURCE_TYPE, (void *)&cf_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_column_family_t *cf = *cf_resource;

  ErlNifBinary key;
  if (!enif_inspect_binary(env, argv[2], &key))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  int result = tidesdb_txn_delete(txn, cf, key.data, key.size);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_txn_rollback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  int result = tidesdb_txn_rollback(txn);

  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  tidesdb_txn_free(txn);
  *txn_resource = NULL;

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_txn_commit(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  int result = tidesdb_txn_commit(txn);

  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  tidesdb_txn_free(txn);
  *txn_resource = NULL;

  return ETDB_ATOM_OK;
}

