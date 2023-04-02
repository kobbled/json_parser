#include <gtest/gtest.h>

extern "C" {
    #include "jsonparser.h"
}

TEST (ParserTest, SimpleDictionary) {
  // Parse simple object
  char json_string[] = "{\"key1\":\"value1\",\"key2\":123}";
  printf("parsing: %s\n", json_string);

  JSONParser parser;
  json_parser_init(&parser, json_string);
  json_parser_parse_next(&parser);

  //root object
  ASSERT_EQ(parser.root->type, JSON_TYPE_OBJECT);
  ASSERT_EQ(parser.root->key, nullptr);
  ASSERT_EQ(parser.root->string_value, nullptr);
  ASSERT_EQ(parser.root->number_value, 0);
  
  // "key1":"value1
  ASSERT_EQ(parser.root->next->type,JSON_TYPE_STRING);
  ASSERT_FALSE(strcmp(parser.root->next->key, "key1"));
  ASSERT_FALSE(strcmp(parser.root->next->string_value, "value1"));
  ASSERT_EQ(parser.root->next->number_value,0);

  // "key2":123
  ASSERT_EQ(parser.root->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_FALSE(strcmp(parser.root->next->next->key, "key2"));
  ASSERT_EQ(parser.root->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->number_value,123);
  ASSERT_EQ(parser.root->next->next->next, nullptr);

}


TEST (ParserTest, SimpleArray) {
  // Parse simple object
  char json_string[] = "[\"value1\",\"value2\",-123,3.14159]";
  printf("parsing: %s\n", json_string);

  JSONParser parser;
  json_parser_init(&parser, json_string);
  json_parser_parse_next(&parser);

  ASSERT_EQ(parser.root->type,JSON_TYPE_ARRAY);
  ASSERT_EQ(parser.root->string_value, nullptr);
  ASSERT_EQ(parser.root->number_value,0);
  // "value1"
  ASSERT_EQ(parser.root->next->type,JSON_TYPE_STRING);
  ASSERT_FALSE(strcmp(parser.root->next->string_value, "value1"));
  ASSERT_EQ(parser.root->next->number_value,0);
  // "value2"
  ASSERT_EQ(parser.root->next->next->type,JSON_TYPE_STRING);
  ASSERT_FALSE(strcmp(parser.root->next->next->string_value, "value2"));
  ASSERT_EQ(parser.root->next->next->number_value,0);
  // 123
  ASSERT_EQ(parser.root->next->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_EQ(parser.root->next->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->next->number_value,-123);
  // 3.14159
  ASSERT_EQ(parser.root->next->next->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_EQ(parser.root->next->next->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->next->next->number_value,3.14159);


}