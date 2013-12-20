
//
// wiki-registry.h
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//


#ifndef __WIKI_REGISTRY__
#define __WIKI_REGISTRY__ 1

#include "list.h"

typedef struct {
  const char *repo;
  const char *href;
  const char *description;
  const char *category;
} package_t;

list_t *wiki_registry(const char *);
list_t *wiki_registry_parse(const char *);

#endif
