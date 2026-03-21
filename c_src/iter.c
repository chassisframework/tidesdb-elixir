#include <erl_nif.h>
#include <tidesdb/db.h>
#include "iter.h"
#include "atoms.h"
#include "resources.h"
#include "errors.h"

ERL_NIF_TERM etdb_iter_new(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_txn_t **txn_resource;
  if (!enif_get_resource(env, argv[0], ETDB_TXN_RESOURCE_TYPE, (void *)&txn_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_txn_t *txn = *txn_resource;

  tidesdb_column_family_t **cf_resource;
  if (!enif_get_resource(env, argv[1], ETDB_CF_RESOURCE_TYPE, (void *)&cf_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_column_family_t *cf = *cf_resource;

  tidesdb_iter_t *iter;
  int result = tidesdb_iter_new(txn, cf, &iter);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  tidesdb_iter_t **iter_resource = enif_alloc_resource(ETDB_ITER_RESOURCE_TYPE, sizeof(tidesdb_iter_t *));
  if (!iter_resource)
    return ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE;
  *iter_resource = iter;

  ERL_NIF_TERM iter_ref = enif_make_resource(env, iter_resource);
  enif_release_resource(iter_resource);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, iter_ref);
}

ERL_NIF_TERM etdb_iter_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  tidesdb_iter_free(iter);
  *iter_resource = NULL;

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_iter_valid(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  if (tidesdb_iter_valid(iter))
    return ETDB_ATOM_TRUE;
  else
    return ETDB_ATOM_FALSE;
}

ERL_NIF_TERM etdb_iter_seek_to_first(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  int result = tidesdb_iter_seek_to_first(iter);
  if (result == TDB_SUCCESS)
    return ETDB_ATOM_OK;
  else
    return etdb_tdb_error_tuple(env, result);
}

ERL_NIF_TERM etdb_iter_seek_to_last(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  int result = tidesdb_iter_seek_to_last(iter);
  if (result == TDB_SUCCESS)
    return ETDB_ATOM_OK;
  else
    return etdb_tdb_error_tuple(env, result);
}

ERL_NIF_TERM etdb_iter_key(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  uint8_t *key;
  size_t key_size;

  int result = tidesdb_iter_key(iter, &key, &key_size);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  /* tidesdb owns `key`, we have to make our own copy */
  ErlNifBinary erl_key;

  if (!enif_alloc_binary(sizeof(uint8_t) * key_size, &erl_key))
    return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_alloc_binary"));

  memcpy(erl_key.data, key, sizeof(uint8_t) * key_size);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, enif_make_binary(env, &erl_key));
}

ERL_NIF_TERM etdb_iter_value(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  uint8_t *value;
  size_t value_size;

  int result = tidesdb_iter_value(iter, &value, &value_size);
  if (result != TDB_SUCCESS)
    return etdb_tdb_error_tuple(env, result);

  /* tidesdb owns `value`, we have to make our own copy */
  ErlNifBinary erl_value;

  if (!enif_alloc_binary(sizeof(uint8_t) * value_size, &erl_value))
    return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_alloc_binary"));

  memcpy(erl_value.data, value, sizeof(uint8_t) * value_size);

  return enif_make_tuple(env, 2, ETDB_ATOM_OK, enif_make_binary(env, &erl_value));
}

ERL_NIF_TERM etdb_iter_next(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  tidesdb_iter_next(iter);

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_iter_prev(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  tidesdb_iter_prev(iter);

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_iter_seek(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  ErlNifBinary key;
  if (!enif_inspect_binary(env, argv[1], &key))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  tidesdb_iter_seek(iter, key.data, key.size);

  return ETDB_ATOM_OK;
}

ERL_NIF_TERM etdb_iter_seek_for_prev(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_iter_t **iter_resource;
  if (!enif_get_resource(env, argv[0], ETDB_ITER_RESOURCE_TYPE, (void *)&iter_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_iter_t *iter = *iter_resource;

  ErlNifBinary key;
  if (!enif_inspect_binary(env, argv[1], &key))
    return ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

  tidesdb_iter_seek_for_prev(iter, key.data, key.size);

  return ETDB_ATOM_OK;
}
