
#include <stdio.h>
#include "wiki-registry.h"

#define DEFAULT_URL "https://github.com/clibs/clib/wiki/Packages"

int
main(void) {
  list_t *pkgs = wiki_registry(DEFAULT_URL);
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(pkgs, LIST_HEAD);
  while ((node = list_iterator_next(it))) {
    wiki_package_t *pkg = (wiki_package_t *) node->val;
    printf("[package]\n"
           "  repo: %s\n"
           "  description: %s\n"
           "  category: %s\n"
           "  href: %s\n"
         , pkg->repo
         , pkg->description
         , pkg->category
         , pkg->href);
    wiki_package_free(pkg);
  }
  list_iterator_destroy(it);
  list_destroy(pkgs);
}
