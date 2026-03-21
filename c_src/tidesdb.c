#include <erl_nif.h>
#include <tidesdb/db.h>
#include <stdint.h>
#include "dynamic_array.h"
#include "atoms.h"
#include "resources.h"
#include "errors.h"
#include "db.h"
#include "cf.h"
#include "txn.h"
#include "iter.h"
#include "etdb_compound.h"

static int load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info) {
  if (etdb_make_resources(env) != 0)
    return 1;

  etdb_make_atoms(env);
  etdb_make_errors(env);

  return 0;
}

static ErlNifFunc nif_funcs[] = {
  /* tdb functions */
  {"open", 2, etdb_open, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"close", 1, etdb_close, ERL_NIF_DIRTY_JOB_IO_BOUND},

  {"create_column_family", 3, etdb_create_column_family, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"get_column_family", 2, etdb_get_column_family, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"list_column_families", 1, etdb_list_column_families, ERL_NIF_DIRTY_JOB_IO_BOUND},

  {"txn_begin", 1, etdb_txn_begin, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"txn_put", 4, etdb_txn_put, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"txn_get", 3, etdb_txn_get, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"txn_delete", 3, etdb_txn_delete, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"txn_rollback", 1, etdb_txn_rollback, ERL_NIF_DIRTY_JOB_IO_BOUND},
  {"txn_commit", 1, etdb_txn_commit, ERL_NIF_DIRTY_JOB_IO_BOUND},

  {"iter_new", 2, etdb_iter_new},
  {"iter_close", 1, etdb_iter_close},
  {"iter_valid", 1, etdb_iter_valid},
  {"iter_seek_to_first", 1, etdb_iter_seek_to_first},
  {"iter_seek_to_last", 1, etdb_iter_seek_to_last},
  {"iter_key", 1, etdb_iter_key},
  {"iter_value", 1, etdb_iter_value},
  {"iter_next", 1, etdb_iter_next},
  {"iter_prev", 1, etdb_iter_prev},

  /* compound multi-operation functions, to avoid jumping back-and-forth between schedulers */
  {"scan", 3, etdb_scan, ERL_NIF_DIRTY_JOB_IO_BOUND},
};

ERL_NIF_INIT(Elixir.TidesDB.NIF, nif_funcs, load, NULL, NULL, NULL)
