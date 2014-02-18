
# wiki-registry.c

  Turn a GitHub Wiki page into a package registry

## Installation

  Install with [clib(1)](https://github.com/clibs/clib):

    $ clib install stephenmathieson/wiki-registry.c

## API

### `list_t *wiki_registry_parse(const char *html)`

  Get a [list](https://github.com/clibs/list) of packages from the given `html`.

### `list_t *wiki_registry_packages(const char *url)`

  Get a [list](https://github.com/clibs/list) of packages from the given GitHub wiki `url`.

### `void wiki_package_free(wiki_package_t *pkg)`

  Free a wiki `pkg`.

## Example

```c
#include <stdio.h>
#include "wiki-registry.h"

int main() {
  list_t *pkgs = wiki_registry("https://github.com/clibs/clib/wiki/Packages");

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
```

## License

(The MIT License)

Copyright (c) 2014 Stephen Mathieson &lt;me@stephenmathieson.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.