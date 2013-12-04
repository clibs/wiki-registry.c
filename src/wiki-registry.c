
//
// wiki-registry.c
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "gumbo.h"
#include "http-get.h"
#include "wiki-registry.h"
#include "list.h"
#include "substr.h"

//
// TODO find dox on gumbo so the node iteration isn't so ugly
//

/**
 * Parse a repo from a package `url`
 */

static char *package_get_repo(const char *url) {
  size_t l = strlen(url);
  int pos = l;
  int slashes = 0;
  for (; pos; pos--) {
    if ('/' == url[pos]) slashes++;
    if (2 == slashes) break;
  }

  char *pathname = substr(url, pos + 1, l);
  return pathname;
}

/**
 * Create a `package` from the given anchor node
 */

static package_t *package_from_wiki_anchor(GumboNode *anchor) {
  package_t *pkg = malloc(sizeof(package_t));

  GumboAttribute* href = gumbo_get_attribute(&anchor->v.element.attributes, "href");

  char *url = strdup(href->value);
  pkg->href = url;
  pkg->repo = package_get_repo(url);

  GumboNode *parent = anchor->parent;
  if (GUMBO_TAG_LI != parent->v.element.tag) {
    free(pkg);
    return NULL;
  }

  GumboVector* children = &parent->v.element.children;
  for (int i = 0; i < children->length; ++i) {
    GumboNode *child = children->data[i];
    if (GUMBO_NODE_TEXT == child->type) {
      // TODO support nested elements (<code>, <em>, etc.)
      char *description = strdup(child->v.text.text);
      pkg->description = substr(description, 3, strlen(description) + 1);
      break;
    }
  }
  return pkg;
}

/**
 * Iterate through all links, adding to `packages` when applicable
 */

static void wiki_registry_iterate_nodes(GumboNode *node, list_t *packages, char *category) {
  if (node->type != GUMBO_NODE_ELEMENT) return;

  if (node->v.element.tag == GUMBO_TAG_A) {
    GumboAttribute* name = gumbo_get_attribute(&node->v.element.attributes, "name");
    if (name) {
      // set the current category for next iteration
      size_t len = strlen(name->value);
      category = realloc(category, len + 1);
      memcpy(category, name->value, len);
      category[len] = 0;
    } else {
      package_t *pkg = package_from_wiki_anchor(node);
      if (pkg) {
        pkg->category = strdup(category);
        list_node_t *pkg_node = list_rpush(packages, list_node_new(pkg));
      }
    }
  } else {
    GumboVector* children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
      wiki_registry_iterate_nodes(children->data[i], packages, category);
    }
  }
}

/**
 * Iterate through all nodes until we find `#wiki-body`, then parse its links
 */

static void wiki_registry_find_body(GumboNode* node, list_t *packages) {
  if (node->type != GUMBO_NODE_ELEMENT) return;

  GumboAttribute *id = gumbo_get_attribute(&node->v.element.attributes, "id");
  if (id && 0 == strcmp("wiki-body", id->value)) {
    // temp category buffer, we'll populate this later
    char *category = malloc(1);
    wiki_registry_iterate_nodes(node, packages, category);
    return;
  }

  GumboVector* children = &node->v.element.children;
  for (int i = 0; i < children->length; ++i) {
    wiki_registry_find_body(children->data[i], packages);
  }
}

/**
 * Get a list of packages from the given GitHub wiki `url`.
 */

list_t *wiki_registry(const char *url) {
  response_t *res = http_get(url);
  if (!res->ok) return NULL;

  GumboOutput* output = gumbo_parse(res->data);

  list_t *pkgs = list_new();
  wiki_registry_find_body(output->root, pkgs);

  gumbo_destroy_output(&kGumboDefaultOptions, output);

  return pkgs;
}
