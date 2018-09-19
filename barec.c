#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
	NORMAL,
	STARTCOMMENT,
	INCCOMMENT,
	ENDCCOMMENT,
	INSTRING,
	ESCAPE,
	INCPPCOMMENT,
	INCLUDE,
	SPACE
} State;

typedef enum
{
	FALSE,
	TRUE
} Bool;

int iscname (int c)
{
	return isalnum (c) || c == '_';
}

// Skips over:
// - #include --> EOL
// - // --> EOL
// - /* ... */
// Replace with space:
// - EOL
// - tabs
// Prints one space only!
// Separate tokens: alnum -> non-alnum, non-alnum -> other non-alnum, 
int main (int argc, char **args)
{
	int c;						// next char from c source
	int prevc;					// previous char to detect token limits
	int ststr;					// string starting character, has to match

	// with ending char
	State oldst = NORMAL;		// old state
	State st = NORMAL;			// state;position in c source
	Bool keephash = FALSE;		// keeping preprocessor directives lets the

	// code to compile

	if (argc > 1)
		keephash = (strcmp (args[1], "keephash") == 0) ? TRUE : FALSE;

	while ((c = getchar ()) != EOF)
	{
		State leftst = st;

		switch (st)
		{
		case NORMAL:
			switch (c)
			{
				// go to special cases like comment, include, string, escape
			case '/':
				st = STARTCOMMENT;
				break;

			case '#':
				if (keephash)
				{
					putchar ('\n');
					putchar (c);
				}
				st = INCLUDE;
				break;

			case '"':
			case '\'':
				if (oldst != SPACE && prevc != 'L')
					putchar (' ');
				putchar (c);
				st = INSTRING;
				ststr = c;
				break;

			case '\\':
				st = ESCAPE;
				break;
				// process whitespaces
			case '\t':
			case '\n':
			case '\r':
			case ' ':
				if (oldst != SPACE)
					putchar (' ');
				leftst = SPACE;
				break;
				// normal chars
			default:			// process token limits
				if (oldst != SPACE)
				{
					// consider adding space because there was none
					if (iscname (prevc) != iscname (c))
					{
						// leave decimal point!
						if (isdigit (prevc) && c == '.')
						{
						}
						else if (prevc == '.' && isdigit (c))
						{
						}
						else
						{
							putchar (' ');
						}
					}
					if (!iscname (prevc) && !iscname (c))
					{
						if (prevc != c)
						{
							// easier to define when not to add space
							if (prevc == '-' && c == '>')
							{
							}
							else if (prevc == '<' && c == '=')
							{
							}
							else if (prevc == '>' && c == '=')
							{
							}
							else if (prevc == '!' && c == '=')
							{
							}
							else if (prevc == '+' && c == '=')
							{
							}
							else if (prevc == '-' && c == '=')
							{
							}
							else if (prevc == '*' && c == '=')
							{
							}
							else if (prevc == '/' && c == '=')
							{
							}
							else if (prevc == '|' && c == '=')
							{
							}
							else if (prevc == '&' && c == '=')
							{
							}
							else if (prevc == '^' && c == '=')
							{
							}
							else if (prevc == '%' && c == '=')
							{
							}
							else
							{
								putchar (' ');
							}
						}
						else
						{
							// easier to define when not to add space
							if (c == '+' || c == '-' || c == '|'
								|| c == '&' || c == ':' || c == '='
								|| c == '<' || c == '>' || c == '.')
							{
							}
							else
							{
								putchar (' ');
							}
						}
					}
				}
				putchar (c);
				break;
			}
			break;

		case STARTCOMMENT:
			switch (c)
			{
			case '/':
				st = INCPPCOMMENT;
				break;
			case '*':
				st = INCCOMMENT;
				break;
			case '\\':
				putchar ('/');
				leftst = NORMAL;
				st = ESCAPE;
				break;
			case ' ':
				putchar ('/');
				st = NORMAL;
				break;
			case '"':
			case '\'':
				putchar ('/');
				putchar (c);
				st = INSTRING;
				ststr = c;
				break;
			default:
				putchar ('/');
				if (c != '=')
					putchar (' ');
				putchar (c);
				st = NORMAL;
				break;
			}
			break;
		case INCCOMMENT:
			switch (c)
			{
			case '*':
				st = ENDCCOMMENT;
				break;
			}
			break;
		case ENDCCOMMENT:
			switch (c)
			{
			case '/':
				st = NORMAL;
				break;
			case '*':
				st = ENDCCOMMENT;
				break;
			case '\\':
				leftst = INCCOMMENT;
				st = ESCAPE;
				break;
			default:
				st = INCCOMMENT;
				break;
			}
			break;
		case INSTRING:
			switch (c)
			{
			case '"':
			case '\'':
				putchar (c);
				if (c == ststr)
					st = NORMAL;
				break;
			case '\\':
				st = ESCAPE;
				break;
			default:
				putchar (c);
				break;
			}
			break;
		case ESCAPE:
			st = oldst;
			if (st == NORMAL || st == INSTRING || (st == INCLUDE && keephash))
			{
				putchar ('\\');
				putchar (c);
			}
			break;
		case INCPPCOMMENT:
			switch (c)
			{
			case '\r':
			case '\n':
				leftst = SPACE;
				st = NORMAL;
				break;
			}
			break;
		case INCLUDE:
			switch (c)
			{
			case '\\':
				st = ESCAPE;
				break;
			case '\r':
			case '\n':
				st = NORMAL;
				if (keephash)
				{
					putchar (c);
					leftst = SPACE;
				}
				break;

			default:
				if (keephash)
					putchar (c);
			}
			break;
		}						// switch(st)
		prevc = c;
		oldst = leftst;
	}							// while((c=getchar()!=EOF)
}
