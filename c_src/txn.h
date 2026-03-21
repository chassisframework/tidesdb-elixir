#ifndef TXN_H_
#define TXN_H_

#include <erl_nif.h>

ERL_NIF_TERM etdb_txn_begin(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_txn_put(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_txn_get(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_txn_delete(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_txn_rollback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM etdb_txn_commit(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

#endif // TXN_H_
