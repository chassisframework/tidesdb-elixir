#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <erl_nif.h>
#include <tidesdb/db.h>

extern ErlNifResourceType
  *ETDB_DB_RESOURCE_TYPE,
  *ETDB_CF_RESOURCE_TYPE,
  *ETDB_TXN_RESOURCE_TYPE,
   *ETDB_ITER_RESOURCE_TYPE;

int etdb_make_resources(ErlNifEnv *env);

#endif // RESOURCES_H_
