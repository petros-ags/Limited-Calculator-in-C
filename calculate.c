#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int readchars = 0;

int special_chars(char ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\n');
}

int Getchar()
{
    int tmp = getchar();
    readchars++;
    if(!isdigit(tmp) && !special_chars(tmp) && tmp != '(' && tmp != ')' && tmp != '+' && tmp != '-' && tmp != '*')
    {
        //Illegal character
        printf("Found illegal character\n");
        exit(-1);
    }
    return tmp;
}

void Ungetchar(int ch)
{
    readchars--;
    ungetc(ch,stdin);
}

int Peekchar()
{
    int c = Getchar();
    Ungetchar(c);
    return c;
}

void whitespace_ignore()
{
    //Eats any white space (that's not \n)
    while(1)
   {
        char c = Peekchar();
        if(c ==' ' || c == '\t')
	{
            Getchar();
        }
	else
	{
            return;
        }
   }
}

int ParseInt()
{
    int res = 0;
    int first = 1;
    while(1)
    {
        char c = Peekchar();
        if(!isdigit(c))
	{
            if(first)
		{
                	//Expected number error
                	printf("Expected number\n");
                	exit(-4);
            	}
		else
		{
                	return res;
            	}
        }
	else
	{
            first = 0;
            res*= 10;
            res += c-'0';
            Getchar();
        }
    }
}

int getTerm(); //Still need this so Calculate_Expression can use it

int Calculate_Expression(int par)
{
    //Expressions always start with a term
    int endterm = 0;
    int midterm = getTerm();
    int sign = 1;
    int lastp = 0; //It's 1 if the last symbol was "*"
    int seeksymbol = 1;
    while(1)
    {
        whitespace_ignore();

        if(seeksymbol)
	{
            char c = Getchar();
            if(c == '+' || c == '-')
	    {
                endterm += sign*midterm;
                sign = -1+2*(c == '+');   //if c == + is true and returns 1 else returns 0,basically i calculate the sign
                midterm = 0;
            }
	    else if(c == '*')
	    {
                lastp = 1;
            }
	    else if(c == '\n' || c == ')')
	    {
                if((c == ')' && par) || (c == '\n' && !par) )
		{
                    endterm += sign*midterm;
                    return endterm;
                }
		else
		{
                    printf("Unexpected ending of expression\n");
                    exit(-5);
                }

            }
	    else
	    {
                //Unexpected char
                printf("Unexpected character when looking for arithmetic symbol\n");
                exit(-2);
            }
            seeksymbol = 0;
        }
	else
	{

            if(lastp)
	    {
                midterm *= getTerm();
                lastp = 0;
            }
	    else
	    {
                midterm = getTerm();
            }

            seeksymbol = 1;
        }
    }
}

int getTerm()
{
    //Term is either a number or a signed number or a parenthesized expression
    whitespace_ignore();
    char c = Getchar();
    int res;
    if(c == '+' || c == '-')
    {
        //signed number
        res = (1-2*(c == '-'))*ParseInt();
    }
    else if(isdigit(c))
    {
        //unsigned number
        Ungetchar(c);
        res = ParseInt();
    }
    else if(c == '(')
    {
        //Parenthesized expression
        res = Calculate_Expression(1);
    }
    else
    {
        //Unexpected character
        printf("Unexpected character when reading term\n");
        exit(-3);
    }

    return res;
}

int main()
{
	while(1)
	{
		printf("Please enter an arithmetic expression\n");
    	printf("Result: %d\n",Calculate_Expression(0));
	}
}
