#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define NORMAL 0
#define STARTCOMMENT 1
#define INCCOMMENT 2
#define ENDCCOMMENT 3
#define INSTRING 4
#define ESCAPE 5
#define INCPPCOMMENT 6
#define INCLUDE 7
#define SPACE 8

int isword(int c) {
	return isalnum(c) || c=='_';
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
int main(int argc,char**args){
    int c; // next char from c source
    int prevc;  // previous char to detect token limits
    int ststr;  // string starting character, has to match with ending char
    int oldst=NORMAL; // old state
    int st=NORMAL; // state;position in c source
	int keephash=0;
	if (argc>1)
		keephash=(strcmp(args[1],"keephash")==0);
    while((c=getchar())!=EOF){
        switch(st){
            case NORMAL:
                switch(c){
                    // go to special cases like comment, include, string, escape
					case'/':
						if(oldst!=SPACE)
							putchar(' ');
						oldst=st;
						st=STARTCOMMENT;
						break;

					case'#':
						if(keephash){
							putchar('\n');
							putchar(c);
							oldst=st;
						}
						st=INCLUDE;
						break;

                    case'"':case'\'':
						if (oldst!=SPACE&&prevc!='L')
							putchar(' ');
						putchar(c);
						oldst=st;
						st=INSTRING;
						ststr=c;
						break;

                    case'\\':oldst=st;st=ESCAPE;break;
                    // process whitespaces
                    case'\t':case'\n':case'\r':case' ':
						if(oldst!=SPACE)putchar(' ');
						oldst=SPACE;
						break;
                    // normal chars
                    default:  // process token limits
                        if (oldst!=SPACE){
							// consider adding space because there was none
                            if(isword(prevc)!=isword(c)){
								if(isdigit(prevc)&&c=='.'){} else
								if(prevc=='.'&&isdigit(c)){} else
								putchar(' ');
							}
                            if(!isword(prevc)&&!isword(c)){
								if (prevc!=c) {
									// easier to define when not to add space
									if(prevc=='-'&&c=='>'){} else
									if(prevc=='<'&&c=='='){} else
									if(prevc=='>'&&c=='='){} else
									if(prevc=='!'&&c=='='){} else
									if(prevc=='+'&&c=='='){} else
									if(prevc=='-'&&c=='='){} else
									if(prevc=='*'&&c=='='){} else
									if(prevc=='/'&&c=='='){} else
									if(prevc=='|'&&c=='='){} else
									if(prevc=='&'&&c=='='){} else
									if(prevc=='^'&&c=='='){} else
									if(prevc=='%'&&c=='='){} else
									putchar(' ');
								} else {
									// easier to define when not to add space
									if(c=='+'||c=='-'||c=='|'||c=='&'||c==':'||c=='='||c=='<'||c=='>'||c=='.'){}
									else putchar(' ');
								}
							}
                        }
						oldst=st;
                        putchar(c);
                        break;
                }
            break;
            case STARTCOMMENT:
                switch(c){
                    case'/':st=INCPPCOMMENT;break;
                    case'*':st=INCCOMMENT;break;
                    case'\\':putchar('/');oldst=NORMAL;st=ESCAPE;break;
                    case' ':putchar('/');st=NORMAL;break;
                    case'"':case'\'':putchar('/');putchar(c);st=INSTRING;ststr=c;break;
					default:
						putchar('/');
						if(c!='=')putchar(' ');
						putchar(c);
						st=NORMAL;
						break;
                }
            break;
            case INCCOMMENT:
                switch(c){
                    case'*':st=ENDCCOMMENT;break;
                    //case'\\':oldst=st;st=ESCAPE;break;
                }
            break;
            case ENDCCOMMENT:
                switch(c){
                    case'/':st=NORMAL;break;
                    case'*':st=ENDCCOMMENT;break;
                    case'\\':oldst=INCCOMMENT;st=ESCAPE;break;
                    default:st=INCCOMMENT;break;
                }
            break;
            case INSTRING:
                switch(c){
                    case'"':case'\'':putchar(c);if(c==ststr)st=NORMAL;break;
                    case'\\':oldst=st;st=ESCAPE;break;
                    default:putchar(c);break;
                }
            break;
            case ESCAPE:
				st=oldst;
				if(st==NORMAL||st==INSTRING||(st==INCLUDE&&keephash)){
					putchar('\\');
					putchar(c);
				}
            break;
            case INCPPCOMMENT:
                switch(c){
                    case'\r':case'\n':oldst=SPACE;st=NORMAL;break;
                }
            break;
            case INCLUDE:
                switch(c){
					case'\\':oldst=INCLUDE;st=ESCAPE;break;
                    case'\r':case'\n':
						st=NORMAL;
						if(keephash){
							putchar(c);
							oldst=SPACE;
						}
						break;

					default:if(keephash)putchar(c);
                }
            break;
        }  // switch(st)
        prevc=c;
    }  // while((c=getchar()!=EOF)
}
