#include <gtest/gtest.h>

extern "C" {
    #include "jsonparser.h"
}

char dict_string[] = "{\"key1\":\"value1\",\"key2\":123}";

class DictionaryTest : public ::testing::Test {
protected:
  JSONParser parser;

  void SetUp() override {
    json_parser_init(&parser, dict_string);
    json_parser_parse_next(&parser);
  }

  void TearDown() override {
    json_parser_reset(&parser);
  }

};

TEST_F(DictionaryTest, root_object) {
  //root object
  ASSERT_EQ(parser.root->type, JSON_TYPE_OBJECT);
  ASSERT_EQ(parser.root->key, nullptr);
  ASSERT_EQ(parser.root->string_value, nullptr);
  ASSERT_EQ(parser.root->number_value, 0);
}

TEST_F(DictionaryTest, key1_object) {
  // "key1":"value1
  ASSERT_EQ(parser.root->next->type,JSON_TYPE_STRING);
  ASSERT_TRUE(strcmp(parser.root->next->key, "key1"));
  ASSERT_TRUE(strcmp(parser.root->next->string_value, "value1"));
  ASSERT_EQ(parser.root->next->number_value,0);
}

TEST_F(DictionaryTest, key2_object) {
  // "key2":123
  ASSERT_EQ(parser.root->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_TRUE(strcmp(parser.root->next->next->key, "key2"));
  ASSERT_EQ(parser.root->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->number_value,123);
  ASSERT_EQ(parser.root->next->next->next, nullptr);
}


char arr_string[] = "[\"value1\",\"value2\",-123,3.14159]";

class ArrayTest : public ::testing::Test {
protected:
  JSONParser parser;

  void SetUp() override {
    json_parser_init(&parser, arr_string);
    json_parser_parse_next(&parser);
  }

  void TearDown() override {
    json_parser_reset(&parser);
  }

};

TEST_F(ArrayTest, Root) {

  ASSERT_EQ(parser.root->type,JSON_TYPE_ARRAY);
  ASSERT_EQ(parser.root->string_value, nullptr);
  ASSERT_EQ(parser.root->number_value,0);
}

TEST_F(ArrayTest, index0_value1) {
  // "value1"
  ASSERT_EQ(parser.root->next->type,JSON_TYPE_STRING);
  ASSERT_TRUE(strcmp(parser.root->next->string_value, "value1"));
  ASSERT_EQ(parser.root->next->number_value,0);
}

TEST_F(ArrayTest, index1_value2) {
  // "value2"
  ASSERT_EQ(parser.root->next->next->type,JSON_TYPE_STRING);
  ASSERT_TRUE(strcmp(parser.root->next->next->string_value, "value2"));
  ASSERT_EQ(parser.root->next->next->number_value,0);
}

TEST_F(ArrayTest, index3_123) {
  // 123
  ASSERT_EQ(parser.root->next->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_EQ(parser.root->next->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->next->number_value,-123);
}

TEST_F(ArrayTest, index3_pi) {
  // 3.14159
  ASSERT_EQ(parser.root->next->next->next->next->type,JSON_TYPE_NUMBER);
  ASSERT_EQ(parser.root->next->next->next->next->string_value, nullptr);
  ASSERT_EQ(parser.root->next->next->next->next->number_value,3.14159);
}

