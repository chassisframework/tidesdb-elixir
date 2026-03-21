#ifndef ERRORS_H_
#define ERRORS_H_

#include <erl_nif.h>

extern ERL_NIF_TERM
  ETDB_ERROR_TUPLE_ENIF_GET_RESOURCE,
  ETDB_ERROR_TUPLE_ENIF_ALLOC_RESOURCE,
  ETDB_ERROR_TUPLE_ENIF_INSPECT_BINARY;

void etdb_make_errors(ErlNifEnv *env);
ERL_NIF_TERM etdb_tdb_error_tuple(ErlNifEnv *env, int error_code);

#endif // ERRORS_H_
