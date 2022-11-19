#pragma once

const int  FESTIVAL_COMMAND_LENGHT = 1024;
const char FESTIVAL_ZALUPA[] =  "echo \"%s\" | festival --language russian --tts";

void festsay (const char* text);

