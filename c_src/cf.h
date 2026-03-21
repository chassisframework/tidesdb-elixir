#ifndef CF_H_
#define CF_H_

#include <erl_nif.h>

ERL_NIF_TERM etdb_create_column_family(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_get_column_family(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_list_column_families(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

#endif // CF_H_
