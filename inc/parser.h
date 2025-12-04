#pragma once

void error();
void ack();
float parse(char *cmd, char delim);
void parse_command(char *cmd);
void g_command(char *cmd);
void m_command(char *cmd);
void parse_loop(); //This is the only one you should call in main.

static inline uint8_t is_delimiter(char c)
{
	return ((c == 'G')
			|| (c == 'M')
			|| (c == 'X')
			|| (c == 'Y')
			|| (c == 'Z')
			|| (c == 'F')
			|| (c == 'I')
			|| (c == 'J'));
}

static inline uint8_t is_whitespace(char c)
{
	return ((c == ' ')
			|| (c == '\n')
			|| (c == '\r'));
}
