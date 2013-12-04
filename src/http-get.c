
//
// http-get.c
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "http-get.h"

//
// TODO clean this up and create a separate repo for it
//

/**
 * HTTP GET write callback
 */

static size_t http_get_cb(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct ResponseData *res = (struct ResponseData *) userp;

  res->data = realloc(res->data, res->size + realsize + 1);
  if (NULL == res->data) {
    fprintf(stderr, "not enough memory!");
    return 0;
  }

  memcpy(&(res->data[res->size]), contents, realsize);
  res->size += realsize;
  res->data[res->size] = 0;

  return realsize;
}

/**
 * Perform an HTTP(S) GET on `url`
 */

response_t *http_get(const char *url) {
  CURL *req = curl_easy_init();

  static struct ResponseData res;
  res.data = malloc(1);
  res.size = 0;

  curl_easy_setopt(req, CURLOPT_URL, url);
  curl_easy_setopt(req, CURLOPT_HTTPGET, 1);

  curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, http_get_cb);
  curl_easy_setopt(req, CURLOPT_WRITEDATA, (void *)&res);
  curl_easy_setopt(req, CURLOPT_USERAGENT, "wiki-registry/0.0.0");

  int c = curl_easy_perform(req);

  curl_easy_getinfo(req, CURLINFO_RESPONSE_CODE, &res.status);
  res.ok = (200 == res.status && CURLE_ABORTED_BY_CALLBACK != c) ? 1 : 0;
  curl_easy_cleanup(req);

  return &res;
}
