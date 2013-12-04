
#include <stdio.h>
#include "wiki-registry.h"

int main() {
  list_t *pkgs = wiki_registry("https://github.com/clibs/clib/wiki/Packages");

  list_node_t *node;
  list_iterator_t *it = list_iterator_new(pkgs, LIST_HEAD);
  while ((node = list_iterator_next(it))) {
    package_t *pkg = (package_t *) node->val;
    printf("[package]\n"
           "  repo: %s\n"
           "  description: %s\n"
           "  category: %s\n"
           "  href: %s\n"
         , pkg->repo
         , pkg->description
         , pkg->category
         , pkg->href);
  }
  list_iterator_destroy(it);
  list_destroy(pkgs);
}
