#include "jsonparser.h"

int main() {
    // Dummy example to fill main for now
    char *json_string = "{\"key1\": \"value1\", \"key2\": \"value2\"}";
    JSONParser parser;
    json_parser_init(&parser, json_string);
    json_parser_parse_next(&parser);

    //free resources
    json_parser_reset(&parser);

    return 0;

}