#include "atoms.h"

ERL_NIF_TERM
  ETDB_ATOM_OK,
  ETDB_ATOM_ERROR,
  ETDB_ATOM_TRUE,
  ETDB_ATOM_FALSE,
  ETDB_ATOM_ENIF_GET_STRING,
  ETDB_ATOM_MEMORY,
  ETDB_ATOM_INVALID_ARGS,
  ETDB_ATOM_NOT_FOUND,
  ETDB_ATOM_IO,
  ETDB_ATOM_CORRUPTION,
  ETDB_ATOM_EXISTS,
  ETDB_ATOM_CONFLICT,
  ETDB_ATOM_TOO_LARGE,
  ETDB_ATOM_MEMORY_LIMIT,
  ETDB_ATOM_INVALID_DB,
  ETDB_ATOM_UNKNOWN,
  ETDB_ATOM_LOCKED,
  ETDB_ATOM_UNKNOWN_ERROR_CODE;


void etdb_make_atoms(ErlNifEnv *env) {
  ETDB_ATOM_TRUE = enif_make_atom(env, "true");
  ETDB_ATOM_FALSE = enif_make_atom(env, "false");
  ETDB_ATOM_OK = enif_make_atom(env, "ok");
  ETDB_ATOM_ERROR = enif_make_atom(env, "error");
  ETDB_ATOM_ENIF_GET_STRING = enif_make_atom(env, "enif_get_string");
  ETDB_ATOM_MEMORY = enif_make_atom(env, "memory");
  ETDB_ATOM_INVALID_ARGS = enif_make_atom(env, "invalid_args");
  ETDB_ATOM_NOT_FOUND = enif_make_atom(env, "not_found");
  ETDB_ATOM_IO = enif_make_atom(env, "io");
  ETDB_ATOM_CORRUPTION = enif_make_atom(env, "corruption");
  ETDB_ATOM_EXISTS = enif_make_atom(env, "exists");
  ETDB_ATOM_CONFLICT = enif_make_atom(env, "conflict");
  ETDB_ATOM_TOO_LARGE = enif_make_atom(env, "too_large");
  ETDB_ATOM_MEMORY_LIMIT = enif_make_atom(env, "memory_limit");
  ETDB_ATOM_INVALID_DB = enif_make_atom(env, "invalid_db");
  ETDB_ATOM_UNKNOWN = enif_make_atom(env, "unknown");
  ETDB_ATOM_LOCKED = enif_make_atom(env, "locked");
  ETDB_ATOM_UNKNOWN_ERROR_CODE = enif_make_atom(env, "unknown_error_code");
}

