#include "resources.h"

ErlNifResourceType
  *ETDB_DB_RESOURCE_TYPE,
  *ETDB_CF_RESOURCE_TYPE,
  *ETDB_TXN_RESOURCE_TYPE,
  *ETDB_ITER_RESOURCE_TYPE;

static void cleanup_txn_resource(ErlNifEnv *env, void *txn_resource) {
  tidesdb_txn_t *txn = *(tidesdb_txn_t **)txn_resource;
  if (txn)
    tidesdb_txn_free(txn);
}

static void cleanup_iter_resource(ErlNifEnv *env, void *iter_resource) {
  tidesdb_iter_t *iter = *(tidesdb_iter_t **)iter_resource;
  if (iter)
    tidesdb_iter_free(iter);
}

int etdb_make_resources(ErlNifEnv *env) {
  ETDB_DB_RESOURCE_TYPE = enif_open_resource_type(env, NULL, "db", NULL, (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER), NULL);
  if (!ETDB_DB_RESOURCE_TYPE)
    return 1;

  ETDB_CF_RESOURCE_TYPE = enif_open_resource_type(env, NULL, "cf", NULL, (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER), NULL);
  if (!ETDB_CF_RESOURCE_TYPE)
    return 1;

  ETDB_TXN_RESOURCE_TYPE = enif_open_resource_type(env, NULL, "txn", cleanup_txn_resource, (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER), NULL);
  if (!ETDB_TXN_RESOURCE_TYPE)
    return 1;

  ETDB_ITER_RESOURCE_TYPE = enif_open_resource_type(env, NULL, "iter", cleanup_iter_resource, (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER), NULL);
  if (!ETDB_ITER_RESOURCE_TYPE)
    return 1;

  return 0;
}


