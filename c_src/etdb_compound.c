/* compound multi-operation functions, to avoid jumping back-and-forth between schedulers */

#include <erl_nif.h>
#include <tidesdb/db.h>
#include "etdb_compound.h"
#include "dynamic_array.h"
#include "atoms.h"
#include "resources.h"
#include "errors.h"

ERL_NIF_TERM etdb_scan(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  tidesdb_t **db_resource;
  if (!enif_get_resource(env, argv[0], ETDB_DB_RESOURCE_TYPE, (void *)&db_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_t *db = *db_resource;

  tidesdb_column_family_t **cf_resource;
  if (!enif_get_resource(env, argv[1], ETDB_CF_RESOURCE_TYPE, (void *)&cf_resource))
    return ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE;
  tidesdb_column_family_t *cf = *cf_resource;

  int max; // -1 for unlimited
  if (!enif_get_int(env, argv[2], &max))
    return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_get_int"));

  tidesdb_txn_t *txn;
  tidesdb_txn_begin(db, &txn);

  tidesdb_iter_t *iter;
  int result = tidesdb_iter_new(txn, cf, &iter);
  if (result != TDB_SUCCESS) {
    tidesdb_txn_free(txn);
    return etdb_tdb_error_tuple(env, result);
  }

  DynamicArray array;

  if (dynamic_array_init(&array, 100) != 0)
    return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "dynamic_array_init"));

  tidesdb_iter_seek_to_first(iter);

  for (int i=0 ; i != max && tidesdb_iter_valid(iter) ; i++) {
    uint8_t *key, *value;
    size_t key_size, value_size;

    int result;
    if ((result = tidesdb_iter_key(iter, &key, &key_size) != TDB_SUCCESS) ||
        (result = tidesdb_iter_value(iter, &value, &value_size) != TDB_SUCCESS))
      return etdb_tdb_error_tuple(env, result);

    /* tidesdb owns `key` and `value`, we have to make our own copy */
    ErlNifBinary erl_key, erl_value;

    if (!enif_alloc_binary(sizeof(uint8_t) * key_size, &erl_key) ||
        !enif_alloc_binary(sizeof(uint8_t) * value_size, &erl_value))
      return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_alloc_binary"));

    memcpy(erl_key.data, key, sizeof(uint8_t) * key_size);
    memcpy(erl_value.data, value, sizeof(uint8_t) * value_size);

    ERL_NIF_TERM pair_term = enif_make_tuple(env, 2, enif_make_binary(env, &erl_key), enif_make_binary(env, &erl_value));

    if (dynamic_array_append(&array, pair_term) != 0)
      return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "dynamic_array_append"));

    tidesdb_iter_next(iter);
  }

  tidesdb_iter_free(iter);
  tidesdb_txn_free(txn);

  return enif_make_list_from_array(env, array.array, array.used);
}
