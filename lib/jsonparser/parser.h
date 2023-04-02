#ifndef JSONPARSER_PARSER_H
#define JSONPARSER_PARSER_H

#include <ctype.h>
#include "jsonparser/types.h"

//skip whitespace in JSONParser
void json_parser_skip_whitespace(JSONParser *parser);

// Parse the next JSON value
void json_parser_parse_next(JSONParser *parser);


#endif /* JSONPARSER_PARSER_H */