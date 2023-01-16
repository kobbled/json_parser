#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define DEBUG 1

// Maximum JSON string length
#define MAX_JSON_STRING_LENGTH 1024

// JSON value types
#define JSON_TYPE_STRING 1
#define JSON_TYPE_NUMBER 2
#define JSON_TYPE_OBJECT 3
#define JSON_TYPE_ARRAY 4
#define JSON_TYPE_TRUE 5
#define JSON_TYPE_FALSE 6
#define JSON_TYPE_NULL 7

// JSON object
typedef struct JSONObject {
  char *key;
  int type;
  char *string_value;
  double number_value;
  struct JSONObject *next;
} JSONObject;

// JSON array
typedef struct JSONArray {
  int type;
  char *string_value;
  double number_value;
  struct JSONArray *next;
} JSONArray;

// JSON parser
typedef struct JSONParser {
  char *json_string;
  int length;
  int index;
  JSONObject *root;
  JSONObject *current_object;
  JSONArray *current_array;
} JSONParser;

// Initialize the JSON parser
void json_parser_init(JSONParser *parser, char *json_string) {
  parser->json_string = json_string;
  parser->length = strlen(json_string);
  parser->index = 0;
  parser->root = NULL;
  parser->current_object = NULL;
  parser->current_array = NULL;
}

//skip whitespace in JSONParser
void json_parser_skip_whitespace(JSONParser *parser) {
  while (parser->index < parser->length && isspace(parser->json_string[parser->index])) {
    parser->index++;
  }
}

// Parse the next JSON value
void json_parser_parse_next(JSONParser *parser) {
  // Skip whitespace
  json_parser_skip_whitespace(parser);

  // Check for end of JSON string
  if (parser->index == parser->length) {
    return;
  }

#ifdef DEBUG
    if (parser->prev_index != parser->index) {
      // printf("prev: %d, curr %d \n", parser->prev_index, parser->index);
      printf("index: %d, char: %c \n", parser->index, parser->json_string[parser->index]);
    }
#endif

  // Parse value based on first character
  char c = parser->json_string[parser->index];

  if (c == '"') {
    // Parse string value
    parser->index++;
    char *start = parser->json_string + parser->index;
    char *end = start;
    while (parser->index < parser->length && *end != '"') {
      parser->index++;
      end++;
    }

    *end = '\0';
    parser->index++;
    
    #ifdef DEBUG
      printf("found string: %s\n", strdup(start));
    #endif

    #ifdef DEBUG
      parser->prev_index = parser->index + 1;
    #endif
    
    // Add value to current object or array
    if (parser->current_object != NULL) {
      parser->current_object->type = JSON_TYPE_STRING;
      parser->current_object->string_value = strdup(start);

      if (parser->current_object->key != NULL) {
        printf("value: %s , index: %d\n", parser->current_object->string_value, parser->index);
      }
    } else if (parser->current_array != NULL) {
      parser->current_array->type = JSON_TYPE_STRING;
      parser->current_array->string_value = strdup(start);

      printf("value: %s , index: %d\n", parser->current_array->string_value, parser->index);
    }
  //parse value to proper JSON_TYPE
  //values of object, array, and string are recursed
  } else if (c == ':') {
    // Skip ':'
    parser->index++;
    // Skip whitespace
    json_parser_skip_whitespace(parser);

    if (isdigit(c) || c == '-') {
      // Parse number value
      char *start = parser->json_string + parser->index;
      char *end = start;
      while (parser->index < parser->length && (isdigit(*end) || *end == '.')) {
        parser->index++;
        end++;
      }
      *end = '\0';
      parser->index++;
      double value = strtod(start, NULL);

      #ifdef DEBUG
        printf("found number: %lf\n", value);
      #endif

      #ifdef DEBUG
        parser->prev_index = parser->index + 1;
      #endif

      // Add value to current object or array
      if (parser->current_object != NULL) {
        parser->current_object->type = JSON_TYPE_NUMBER;
        parser->current_object->number_value = value;
        printf("value: %lf , index: %d\n", parser->current_object->number_value, parser->index);
      } else if (parser->current_array != NULL) {
        parser->current_array->type = JSON_TYPE_NUMBER;
        parser->current_array->number_value = value;
        printf("value: %lf , index: %d\n", parser->current_array->number_value, parser->index);
      }
    } else if (c == 't') {
      // Parse true value
      parser->index += 4;

      // Add value to current object or array
      if (parser->current_object != NULL) {
        parser->current_object->type = JSON_TYPE_TRUE;
      } else if (parser->current_array != NULL) {
        parser->current_array->type = JSON_TYPE_TRUE;
      }
      printf("value: true , index: %d\n", parser->index);

    } else if (c == 'f') {
      // Parse false value
      parser->index += 5;

      // Add value to current object or array
      if (parser->current_object != NULL) {
        parser->current_object->type = JSON_TYPE_FALSE;
      } else if (parser->current_array != NULL) {
        parser->current_array->type = JSON_TYPE_FALSE;
      }

      printf("value: false , index: %d\n", parser->index);

    } else if (c == 'n') {
      // Parse null value
      parser->index += 4;

      // Add value to current object or array
      if (parser->current_object != NULL) {
        parser->current_object->type = JSON_TYPE_NULL;
      } else if (parser->current_array != NULL) {
        parser->current_array->type = JSON_TYPE_NULL;
      }

      printf("value: null , index: %d\n", parser->index);
    } else {
      // parse if object, array, or string
      json_parser_parse_next(parser);
    }

  } else if (c == '{') {
    // Parse object
    parser->index++;

    // Create new object
    JSONObject *object = malloc(sizeof(JSONObject));
    object->key = NULL;
    object->type = JSON_TYPE_OBJECT;
    object->string_value = NULL;
    object->number_value = 0;
    object->next = NULL;

    // Add object to root or current object
    if (parser->current_object != NULL) {
      object->key = parser->current_object->key;
      parser->current_object->key = NULL;
      object->next = parser->current_object->next;
      parser->current_object->next = object;
      parser->current_object = object;
    } else {
      parser->root = object;
      parser->current_object = object;
    }

    // Parse object properties
    while (parser->index < parser->length && parser->json_string[parser->index] != '}') {
      // Parse property key
      json_parser_parse_next(parser);
      char *key = parser->current_object->string_value;
      parser->current_object->string_value = NULL;

      #ifdef DEBUG
        if (parser->prev_index != parser->index) {
      printf("key: %s , index: %d\n", key, parser->index);
        }
      #endif

      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Parse property value
      parser->current_object->key = key;
      json_parser_parse_next(parser);
      parser->current_object->key = NULL;

      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // if ',' skip to next key/value pair
      if (parser->json_string[parser->index] == ',') {
        parser->index++;
      }
    }

    // Skip '}'
    parser->index++;

    #ifdef DEBUG
      parser->prev_index = parser->index + 1;
    #endif

  } else if (c == '[') {
    // Parse array
    parser->index++;

    // Create new array
    JSONArray *array = malloc(sizeof(JSONArray));
    array->type = JSON_TYPE_ARRAY;
    array->string_value = NULL;
    array->number_value = 0;
    array->next = NULL;

    // Add array to root or current object
    if (parser->current_array != NULL) {
      array->next = parser->current_array->next;
      parser->current_array->next = array;
      parser->current_array = array;
    } else {
      parser->current_array = array;
    }

    // Parse array elements
    while (parser->index < parser->length && parser->json_string[parser->index] != ']') {
      // Parse array element
      json_parser_parse_next(parser);

      // Create new element
      JSONArray *element = malloc(sizeof(JSONArray));
      element->type = parser->current_array->type;
      element->string_value = parser->current_array->string_value;
      element->number_value = parser->current_array->number_value;
      element->next = NULL;

      // Add element to array
      element->next = parser->current_array->next;
      parser->current_array->type = JSON_TYPE_ARRAY;
      parser->current_array->string_value = NULL;
      parser->current_array->number_value = 0;
      parser->current_array->next = element;

      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Skip ',' separator
      if (parser->index < parser->length && parser->json_string[parser->index] == ',') {
        parser->index++;
      }
    }

    // Skip ']'
    parser->index++;

    #ifdef DEBUG
      parser->prev_index = parser->index + 1;
    #endif

  }
}


// Test the JSON parser
void test_json_parser() {
  // Parse simple object
  char json_string[] = "{\"key1\":\"value1\",\"key2\":123}";
  // char json_string[] = "{\"key1\":\"value1\",\"key2\":\"value2\"}";
  printf("parsing: %s\n", json_string);

  JSONParser parser;
  json_parser_init(&parser, json_string);
  json_parser_parse_next(&parser);
  assert(parser.root->type == JSON_TYPE_OBJECT);
  assert(parser.root->next == NULL);
  assert(parser.root->key == NULL);
  assert(parser.root->string_value == NULL);
  assert(parser.root->number_value == 0);
  assert(parser.root->next->type == JSON_TYPE_STRING);
  assert(strcmp(parser.root->next->key, "key1") == 0);
  assert(strcmp(parser.root->next->string_value, "value1") == 0);
  assert(parser.root->next->number_value == 0);
  assert(parser.root->next->next->type == JSON_TYPE_NUMBER);
  assert(strcmp(parser.root->next->next->key, "key2") == 0);
  assert(parser.root->next->next->string_value == "value2");
  assert(parser.root->next->next->number_value == 0);
  assert(parser.root->next->next->next == NULL);

  // // Parse simple array
  // strcpy(json_string, "[\"value1\",123]");
  // json_parser_init(&parser, json_string);
  // json_parser_parse_next(&parser);
  // assert(parser.root->type == JSON_TYPE_ARRAY);
  // assert(parser.root->next == NULL);
  // assert(parser.root->key == NULL);
  // assert(parser.root->string_value == NULL);
  // assert(parser.root->number_value == 0);
  // assert(parser.root->next->type == JSON_TYPE_STRING);
  // assert(parser.root->next->key == NULL);
  // assert(strcmp(parser.root->next->string_value, "value1") == 0);
  // assert(parser.root->next->number_value == 0);
  // assert(parser.root->next->next->type == JSON_TYPE_NUMBER);
  // assert(parser.root->next->next->key == NULL);
  // assert(parser.root->next->next->string_value == NULL);
  // assert(parser.root->next->next->number_value == 123);
  // assert(parser.root->next->next->next == NULL);
}

void main() {
  test_json_parser();
}
