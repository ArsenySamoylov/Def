#pragma once

#include "DefTree.h"

const int MAX_WORD_LENGTH = 16;
const int MIN_NUMBER_OF_TOKENS = 20;

typedef DefNode Token;

int Tokenizer (Token** tokens_arr, const char* expression);

DefNode* GetG (Token* token_arr, int number_of_tokens);