%{
  #include <stdio.h>
  #include <stdlib.h> 
  int count = 0;
  int line_num = 0;
%} 

delim [" "\n\t] 
whitespace {delim}+ 
operator <|>|\:|=|\+|-|\*|;|\/|<=|>=|:=|%|#|\"|\'|&
keyword [aA][uU][tT][oO]|[sS][hH][oO][rR][tT]|[iI][nN][tT]|[lL][oO][nN][gG]|[fF][lL][oO][aA][tT]|[dD][oO][uU][bB][lL][eE]|[cC][hH][aA][rR]|[sS][tT][rR][uU][cC][tT]|[uU][nN][iI][oO][nN]|[eE][nN][uU][mM]|[tT][yY][pP][eE][dD][eE][fF]|[cC][oO][nN][sS][tT]|[uU][nN][sS][iI][gG][nN][eE][dD]|[sS][iI][gG][nN][eE][dD]|[eE][xX][tT][eE][rR][nN]|[rR][eE][gG][iI][sS][tT][eE][rR]|[sS][tT][aA][tT][iI][cC]|[vV][oO][lL][aA][tT][iI][lL][eE]|[vV]][oO][iI][dD]|[iI][fF]|[eE][lL][sS][eE]|[sS][wW][iI][tT][cC][hH]|[cC][aA][sS][eE]|[fF][oO][rR]|[dD][oO]|[wW][hH][iI][lL][eE]|[gG][oO][tT][oO]|[cC][oO][nN][tT][iI][nN][uU][eE]|[bB][rR][eE][aA][kK]|[dD][eE][fF][aA][uU][lL][tT]|[sS][iI][zZ][eE][oO][fF]|[rR][eE][tT][uU][rR][nN]
delimiter [,\.;\(\)\{\}\[\]]
digit ([0-9])+
letter [A-Za-z]
identifier {letter}({letter}|{digit})*
number {digit}+(\.{digit}+)?(E[+\-]?{digit}+)?
%% 

{keyword} {line_num++;printf("%d\t(keyword,‘%s’)\n",line_num,yytext);}
{operator} {line_num++;printf("%d\t(operator,‘%s’)\n",line_num,yytext); }
{delimiter} {line_num++;printf("%d\t(delimiter,‘%s’)\n",line_num,yytext);}
{number} {line_num++;printf("%d\t(number,‘%s’)\n",line_num,yytext);}
{identifier} {line_num++;printf("%d\t(identifier,‘%s’)\n",line_num,yytext);} 
{whitespace} { /* do    nothing*/ } 
"//".* { /* do    nothing*/ }
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] { /* do    nothing*/ }

%% 
void main() {
  printf("THE LEXICAL ANALYSIS GENERATOR LEX:\n");
  printf("\n");
  yyin=fopen("test.txt","r"); 
  yylex(); /* start the analysis*/ 
  fclose(yyin);
}
int yywrap() 
{ 
  return 1; 
}