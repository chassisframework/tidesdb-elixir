#include <tidesdb/db.h>
#include "atoms.h"
#include "errors.h"

ERL_NIF_TERM
  ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE,
  ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE,
  ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

void etdb_make_errors(ErlNifEnv *env) {
  ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE = enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_alloc_resource"));
  ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE = enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_get_resource"));
  ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY = enif_make_tuple(env, 2, ETDB_ATOM_ERROR, enif_make_atom(env, "enif_inspect_binary"));
}

ERL_NIF_TERM etdb_tdb_error_tuple(ErlNifEnv *env, int error_code) {
  ERL_NIF_TERM error;

  switch (error_code) {
    case TDB_ERR_MEMORY:
      error = ETDB_ATOM_MEMORY;
      break;
    case TDB_ERR_INVALID_ARGS:
      error = ETDB_ATOM_INVALID_ARGS;
      break;
    case TDB_ERR_NOT_FOUND:
      error = ETDB_ATOM_NOT_FOUND;
      break;
    case TDB_ERR_IO:
      error = ETDB_ATOM_IO;
      break;
    case TDB_ERR_CORRUPTION:
      error = ETDB_ATOM_CORRUPTION;
      break;
    case TDB_ERR_EXISTS:
      error = ETDB_ATOM_EXISTS;
      break;
    case TDB_ERR_CONFLICT:
      error = ETDB_ATOM_CONFLICT;
      break;
    case TDB_ERR_TOO_LARGE:
      error = ETDB_ATOM_TOO_LARGE;
      break;
    case TDB_ERR_MEMORY_LIMIT:
      error = ETDB_ATOM_MEMORY_LIMIT;
      break;
    case TDB_ERR_INVALID_DB:
      error = ETDB_ATOM_INVALID_DB;
      break;
    case TDB_ERR_UNKNOWN:
      error = ETDB_ATOM_UNKNOWN;
      break;
    case TDB_ERR_LOCKED:
      error = ETDB_ATOM_LOCKED;
      break;
    default:
      error = ETDB_ATOM_UNKNOWN_ERROR_CODE;
      break;
    }

  return enif_make_tuple(env, 2, ETDB_ATOM_ERROR, error);
}

