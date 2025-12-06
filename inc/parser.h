#pragma once

void error(); //Send an "error:\n" message
void ack(); //Send an "ok\n" acknowledgement message
float parse(char *cmd, char delim); //Parses a float value after a delimiter character.
void parse_command(char *cmd); //Calls either g_command() or m_command()
void g_command(char *cmd); //Parses g-commands and puts actions on queue
void m_command(char *cmd); //Currently a no-op
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
