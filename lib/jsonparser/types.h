#ifndef JSONPARSER_TYPES_H
#define JSONPARSER_TYPES_H

#include <string.h>
#include <stdlib.h>

// JSON object
typedef struct JSONObject {
  char *key;
  int type;
  char *string_value;
  double number_value;
  struct JSONObject *next;
} JSONObject;

// JSON parser
typedef struct JSONParser {
  char *json_string;
  int length;
  int index;
  int prev_index;
  JSONObject *root;
  JSONObject *current_object;
  JSONObject *current_array;
} JSONParser;


// Initialize the JSON parser
void json_parser_init(JSONParser *parser, char *json_string);

//Initialize a new JSONObject
JSONObject* json_object_init(int type);

#endif /* JSONPARSER_TYPES_H */