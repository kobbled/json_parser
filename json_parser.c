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
void json_parser_init(JSONParser *parser, char *json_string) {
  parser->json_string = json_string;
  parser->length = strlen(json_string);
  parser->index = 0;
  parser->prev_index = -1;
  parser->root = NULL;
  parser->current_object = NULL;
  parser->current_array = NULL;
}

//Initialize a new JSONObject
JSONObject* json_object_init(int type) {
  JSONObject *object = malloc(sizeof(JSONObject));
  object->key = NULL;
  object->type = type;
  object->string_value = NULL;
  object->number_value = 0;
  object->next = NULL;

  return object;
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
  //recurse out to parent, as string might be a key, and needs to be handled
  //to move to the value
  } else if (c == ':') {
    // Skip ':'
    parser->index++;
    // Skip whitespace
    json_parser_skip_whitespace(parser);

    json_parser_parse_next(parser);

  } else if (isdigit(c) || c == '-') {
      // Parse number value
      char *start = parser->json_string + parser->index;
      char *end = start;
      while (parser->index < parser->length && (isdigit(*end) || *end == '-' || *end == '.')) {
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

  } else if (c == '{') {
    // Parse object
    parser->index++;

    // Create new object
    JSONObject *object = json_object_init(JSON_TYPE_OBJECT);

    // Add object to root
    if (parser->root == NULL) {
      parser->root = object;
    }
    
    //switch current object to next object
    if (parser->current_object != NULL) {
      //set as next object in linked list/chain
      parser->current_object->next = object;
      //set as current object in parser
      parser->current_object = object;
    } else if (parser->current_array != NULL) {
      //link this object to parent array
      parser->current_array->next = object;
      //set current_array to null to indicate we are in an object
      parser->current_array = NULL;
      //set current_object to this object
      parser->current_object = object;
    } else {
      //switch to object from null or array
      parser->current_object = object;
      parser->current_array = NULL;
    }

    // Parse object properties
    while (parser->index < parser->length && parser->json_string[parser->index] != '}') {
      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Create new element
      JSONObject *element = json_object_init(JSON_TYPE_OBJECT);

      //link to new element in object chain
      parser->current_object->next = element;
      parser->current_object = element;

      // Parse property key
      json_parser_parse_next(parser);
      char *key = parser->current_object->string_value;
      //copy key to current object->key
      parser->current_object->key = key;
      //null string value
      parser->current_object->string_value = NULL;

      #ifdef DEBUG
        if (parser->prev_index != parser->index) {
      printf("key: %s , index: %d\n", key, parser->index);
        }
      #endif

      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Parse property value
      json_parser_parse_next(parser);

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
    JSONObject *array = json_object_init(JSON_TYPE_ARRAY);

    // Add array to root
    if (parser->root == NULL) {
      parser->root = array;
    }

    // Add array to root or current object
    if (parser->current_array != NULL) {
      //link to parent node
      parser->current_array->next = array;
      //switch to this node
      parser->current_array = array;
    } else if (parser->current_object != NULL) {
      //link to parent node
      parser->current_object->next = array;
      //set current_object to null to indicate we are in an array
      parser->current_object = NULL;
      //switch to this node
      parser->current_array = array;
    } else {
      //switch to array from object
      parser->current_array = array;
      parser->current_object = NULL;
    }

    // Parse array elements
    while (parser->index < parser->length && parser->json_string[parser->index] != ']') {
      
      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Create new element
      JSONObject *element = json_object_init(JSON_TYPE_ARRAY);

      //add to parent node
      parser->current_array->next = element;
      //index to this object
      parser->current_array = element;
      
      // Parse array element
      json_parser_parse_next(parser);

      // Skip whitespace
      json_parser_skip_whitespace(parser);

      // Skip ',' separator
      if (parser->json_string[parser->index] == ',') {
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
