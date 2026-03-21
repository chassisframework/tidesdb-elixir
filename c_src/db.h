#ifndef DB_H_
#define DB_H_

#include <erl_nif.h>

ERL_NIF_TERM etdb_open(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

#endif // DB_H_
