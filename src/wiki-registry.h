
//
// wiki-registry.h
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//


#ifndef WIKI_REGISTRY_H
#define WIKI_REGISTRY_H 1

#include "list/list.h"

typedef struct {
  const char *repo;
  const char *href;
  const char *description;
  const char *category;
} wiki_registry_package_t;

list_t *
wiki_registry(const char *);

list_t *
wiki_registry_parse(const char *);

#endif
