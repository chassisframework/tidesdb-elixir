#ifndef ITER_H_
#define ITER_H_

#include <erl_nif.h>

ERL_NIF_TERM etdb_iter_new(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_valid(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_seek_to_first(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_seek_to_last(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_key(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_value(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_next(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_prev(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_seek(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_iter_seek_for_prev(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

#endif // ITER_H_
