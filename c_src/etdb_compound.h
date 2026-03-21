/* compound multi-operation functions, to avoid jumping back-and-forth between schedulers */

#ifndef ETDB_COMPOUND_H_
#define ETDB_COMPOUND_H_

#include <erl_nif.h>

ERL_NIF_TERM etdb_scan(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

#endif // ETDB_COMPOUND_H_
