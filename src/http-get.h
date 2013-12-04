
//
// http-get.h
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#ifndef _HTTP_GET_H_
#define _HTTP_GET_H_ 1

struct ResponseData {
  char *data;
  size_t size;
  long status;
  int ok;
};

typedef struct ResponseData response_t;

response_t *http_get(const char *url);

#endif
