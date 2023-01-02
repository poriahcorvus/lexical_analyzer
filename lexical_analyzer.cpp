/*
允许的字符：
数字：0-9
字母：a-z, A-Z
符号：< > : = + - * ( ) ; , { } / <= >= := // 多行注释符号 % # " ' . &
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#define NUM "NUMBER"
#define KEY "KEYWORD"
#define IDN "IDENTIFIER"
#define ANN "ANNOTATION"

#define ERROR_NUM 0
#define ERROR_NUM_POINT 1
#define ERROR_CHAR 2
#define ERROR_OP 3

#define WRONG_NUM "A wrong number."
#define WRONG_NUM_POINT "There must be a digit behind the point."
#define WRONG_CHAR "There must be a digit , operators'+' or '-' behind E."
#define WRONG_OP "There must be a digit behind the operator."

//C语言关键字
string cword[32] = {
    "auto", "short", "int", "long", "float", "double", "char", "struct",
    "union", "enum", "typedef", "const", "unsigned", "signed", "extern", "register",
    "static", "volatile", "void", "if", "else", "switch", "case","for",
    "do", "while", "goto", "continue", "break", "default", "sizeof", "return"
};

class Token{
public:
    vector <string> keywords;//关键字
    vector <string> lines;  //以行为单位存储
    vector <string> words;  //单词
    vector <string> types;  //每个单词对应记号
    vector <string> errors; //错误


public:
    Token();
    ~Token();
    void ReadOneLine(); //以行为单位读入
    void AnalyseOneWord();  //词法分析
    int IsLetter(char x);   //是否是字母
    int IsDigit(char x);    //是否是数字
    int IsKeyword(string x);    //是否是关键字
    char IsSpace(int& i, string line);  //是否是空格
    void TheError(int linenum, int pos, int wrongtype); //错误处理
    void KeyList(); //关键字列表

};

Token::Token(){
}

Token::~Token(){
}

void Token::ReadOneLine(){
    ifstream fp;
    fp.open("test.txt", ios::in);
    if(!fp.is_open()){
        return;
    }
    char temp[10240];
    while(!fp.eof()){
        fp.getline(temp, 10240);
        lines.push_back(temp);        
    }
    fp.close();
}

void Token::AnalyseOneWord(){
    string line;
    int state = 0;

    for(int i = 0; i < lines.size(); i ++){
        line = lines[i];
        string word = "";
        for(int j = 0; j < line.size(); j ++){
            char current = line[j];
            switch(state){
                case 0:
                {
                    word = "";
                    current = IsSpace(j, line);
                    if(j >= line.size())    break;
                    word += current;

                    if(IsLetter(current)){
                        state = 1;
                        words.push_back(word);
                        types.push_back(IDN);
                        break;
                    }

                    if(IsDigit(current)){
                        state = 2;
                        words.push_back(word);
                        types.push_back(NUM);
                        break;
                    }

                    switch(current){
                        case '<':
                        {
                            state = 8;
                            if(j == line.size()-1)
                                state = 0;
                            words.push_back(word);
                            types.push_back(word);
                            break;
                        }
                        case '>':
                        {
                            state = 9;
                            if(j == line.size()-1)
                                state = 0;
                            words.push_back(word);
                            types.push_back(word);
                            break;
                        }
                        case ':':
                        {
                            state = 10;
                            if(j == line.size()-1)
                                state = 0;
                            words.push_back(word);
                            types.push_back(word);
                            break;
                        }
                        case '#':
                        case '%':
                        case '=':
                        case '+':
                        case '-':
                        case '*':
                        case '(':
                        case ')':
                        case ';':
                        case ',':
                        case '"':
                        case '\'':
                        case '.':
                        case '&':
                        case '{':
                        case '}':
                        {
                            state = 0;
                            words.push_back(word);
                            types.push_back(word);
                            break;
                        }
                        case '/':
                        {
                            state = 11;
                            words.push_back(word);
                            types.push_back(word);
                            break;
                        }
                        default:
                        {
                            state = 14;
                            word.pop_back();
                            break;
                        }
                    }

                    break;
                    
                }
                case 1:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)||IsLetter(current)){
                        state = 1;
                        word += current;
                        words.push_back(word);
                        types.push_back(IDN);
                    }
                    else{
                        j --;
                        state = 0;
                        words.push_back(word);
                        if(IsKeyword(word))
                            types.push_back(KEY);
                        else
                            types.push_back(IDN);
                    }
                    break;
                }
                case 2:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 2;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                        break;
                    }
                    switch(current){
                        case '.':
                        {
                            state = 3;
                            word += current;
                            words.push_back(word);
                            types.push_back(NUM);
                            break;
                        }
                        case 'E':
                        {
                            state = 5;
                            word += current;
                            words.push_back(word);
                            types.push_back(NUM);
                            break;
                        }
                        default:
                        {
                            j --;
                            state = 0;
                            words.push_back(word);
                            types.push_back(NUM);
                            break;
                        }
                    }
                    break;
                }
                case 3:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 4;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else{
                        //错误
                        state = 0;
                        TheError(i, j, ERROR_NUM_POINT);
                    }
                    break;
                }
                case 4:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 4;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else if(current == 'E'){
                        state = 5;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else{
                        j --;
                        state = 0;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    break;
                }
                case 5:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 7;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else if(current == '+' || current == '-'){
                        state = 6;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else{
                        //错误
                        j --;
                        TheError(i, j, ERROR_CHAR);
                    }
                    break;
                }
                case 6:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 7;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else{
                        //错误
                        j --;
                        TheError(i, j, ERROR_OP);
                    }
                    break;
                }
                case 7:
                {
                    words.pop_back();
                    types.pop_back();
                    if(IsDigit(current)){
                        state = 7;
                        word += current;
                        words.push_back(word);
                        types.push_back(NUM);
                    }
                    else{
                        j --;
                        words.push_back(word);
                        types.push_back(NUM);
                        state = 0;
                    }
                    break;
                }
                case 8:
                {
                    words.pop_back();
                    types.pop_back();
                    if(current == '=' || current == '>'){
                        word += current;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    else{
                        j --;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    break;
                }
                case 9:
                {
                    words.pop_back();
                    types.pop_back();
                    if(current == '='){
                        word += current;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    else{
                        j --;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    break;
                }
                case 10:
                {
                    words.pop_back();
                    types.pop_back();
                    if(current == '='){
                        word += current;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    else{
                        j --;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    break;
                }
                case 11:
                {
                    words.pop_back();
                    types.pop_back();
                    if(current == '/'){
                        word += current;
                        words.push_back("//");
                        types.push_back(word);
                        j = line.size()-1;
                        state = 0;
                    }
                    else if(current == '*'){
                        state = 12;
                        word += current;
                    }
                    else{
                        j --;
                        words.push_back(word);
                        types.push_back(word);
                        state = 0;
                    }
                    break;
                }
                case 12:
                {
                    if(current == '*')
                        state = 13;
                    else
                        state = 12;
                    break;
                }
                case 13:
                {
                    if(current == '/'){
                        word = word + "*/";
                        words.push_back("/**/");
                        types.push_back(ANN);
                        state = 0;
                    }
                    else
                        state = 12;
                    break;
                }
                case 14:
                {
                    TheError(i, --j, ERROR_CHAR);
                    break;
                }

            }
        }
    }
}

int Token::IsDigit(char x)
{
    if(x >= '0' && x <= '9')
        return 1;
    else
        return 0;
}

int Token::IsLetter(char x)
{
    if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
        return 1;
    else
        return 0;
}

int Token::IsKeyword(string x)
{
    for(int i = 0; i < keywords.size(); i ++){
        if(x == keywords[i])
            return 1;
    }
    return 0;
}

char Token::IsSpace(int& i, string line)
{
    char cur = line[i];
    while(cur == ' '|| cur == '\t'){
        i ++;
        if(i >= line.size())
            return cur;
        cur = line[i];
    }
    return cur;
}

void Token::TheError(int linenum, int pos, int wrongtype)
{
    string reason = "Error:";
    reason += "\tLine:";
    reason += to_string(linenum+1);//从1开始计数
    reason += "\tPosition:";
    reason += to_string(pos+1);//从1开始计数
    reason += "\n";

    switch(wrongtype)
    {
        case ERROR_NUM:
        {
            reason += WRONG_NUM;
            reason += "\n";
            break;
        }
        case ERROR_NUM_POINT:
        {
            reason += WRONG_NUM_POINT;
            reason += "\n";
            break;
        }
        case ERROR_CHAR:
        {
            reason += WRONG_CHAR;
            reason += "\n";
            break;
        }
        case ERROR_OP:
        {
            reason += WRONG_OP;
            reason += "\n";
            break;
        }
    }
    errors.push_back(reason);
}

void Token::KeyList(){
    for(int i = 0; i < 32; i ++)
        keywords.push_back(cword[i]);
}

int main()
{
    Token t;
    t.KeyList();//准备关键字
    t.ReadOneLine();
    t.AnalyseOneWord();

    //计算记号类别数量
    int typenum = 0;
    int typeflag = 0;
    for(int i = 0; i < t.types.size(); i ++){
        for(int j = 0; j < i; j ++){
            if(t.types[i] == t.types[j]){
                typeflag = 1;
                break;
            } 
        }
        if(typeflag == 0)
            typenum ++;
        typeflag = 0;
    }

    //输出结果
    cout << "TOTAL:" << endl;
    cout << "LINES:" << t.lines.size() << endl;
    cout << "WORDS:" << t.words.size() << endl;
    cout << "TYPES:" << typenum << endl;

    cout << "\nDETAILS:" << endl;
    for(int i = 0; i < t.words.size(); i ++){
        cout << t.words[i] << ":\t" << t.types[i] << endl;
    }

    if(t.errors.size()){
        cout << "\nERRORLIST:" << endl;
        for(int i = 0; i < t.errors.size(); i ++){
            cout << t.errors[i] << endl;
        }
    }
    else
        cout << "\nThere is no error in the code." << endl;

    
}