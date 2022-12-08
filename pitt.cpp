// TODO LIST:
// TODO: Add Linux Syscalls Support
// TODO: Fix a little bit of parsing to add // to strings
// TODO: Add different value stack-pointers (@i16,@i32) [@i64 is OPTIONAL we could implement it in pitt too]
// TODO: Add float support [Not as main] (@f32,@f64) [OPTIONAL]
// TODO: Add operation type provider (Semi Generics Support) for operations with different type of variables
//       Syntax = "value value <Type to operate stack>(if none provided @i32 is operated) <operator>"
// TODO: Add support for `macros`
// TODO: Add argc and argv support
// TODO: Add variable support
// TODO: Add function support (Add `main` entry check if possible) [INFO: Arguments are not directly defined they are popped from the respective stack]
// TODO: Add return statements and return type check for functions
// TODO: Add variable scope if it is possible
// TODO: Add dynamic memory which can be released [maybe!?] (Keyword: `memory name size end`)
// TODO: Add `import` keyword
// TODO: Add Structures

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "./token.hpp"
#include <stack>
using namespace std;

#define PITT_EXT "pitt"
#define RECURSION_LIMIT 1000

void help()
{
    cout << "PITT: " << endl;
    cout << "pitt (command) <filename> [subcmd]" << endl;
    cout << "COMMAND: " << endl;
    cout << "       run     simulates the file" << endl;
    cout << "       dump    dumps all the tokens" << endl;
    cout << "SUBCMD: " << endl;
    cout << "       timeit  Times exection time" << endl;
}

// DONE: Added a padding of P_EOT (refererring to END_OF_TOKEN) to manage extra spaces on words_count()
// while parsing strings
int words_count(const char *filename)
{
    int _count = 0;
    ifstream file{filename};
    string temp{};

    while (file >> temp)
    {
        _count++;
    }

    file.close();
    return _count;
}

void check_file_ext(const char *filename)
{
    size_t j{};
    for (; j < strlen(filename); j++)
    {
        if (filename[j] == '.' && filename[j + 1] == '/')
        {
        }
        else if (filename[j] == '.')
        {
            break;
        }
    }
    char buffer[strlen(filename) - (j + 1)] = {'\0'};
    uint8_t buf_c = 0;
    j++;
    for (; j < strlen(filename); j++)
    {
        buffer[buf_c++] = filename[j];
    }
    buffer[buf_c++] = '\0';
    if (strcmp(buffer, PITT_EXT) != 0)
    {
        printf("pitt: \033[0;31mfatal error:\033[0;37m invalid file format provided `%s`\n\n", filename);
        exit(1);
    }
}

void check_file_val(const char *filename)
{
    ifstream file{filename};

    if (file.is_open())
    {
        file.close();
    }
    else
    {
        printf("forth:\033[0;31m fatal error:\033[0;37m can not open file to read `%s`\n\n", filename);
        exit(1);
    }
}

bool isnum(string &text)
{
    bool state = false;

    if (text.size() > 0)
    {
        size_t _c = {};
        for (size_t i = 0; i < text.size(); i++)
        {
            if (i == 0 && (text[i] == '-' || text[i] == '+') && text.size() > 1)
            {
                _c++;
            }
            else if (text[i] >= '0' && text[i] <= '9')
            {
                _c++;
            }
        }

        if (_c == text.size())
        {
            state = true;
        }
    }

    return state;
}

PittType fetch_type(string &text)
{
    PittType rettype = P_UNDEF;
    for (auto it : usable_map)
    {
        if (text == it.first)
        {
            rettype = it.second;
            break;
        }
    }
    return rettype;
}

int parse_words_in_text(string &text)
{
    istringstream serve(text);
    string temp = {};
    int count = 0;

    while (serve >> temp)
    {
        count++;
    }
    return count;
}

// FIXME: ADD SUPPORT FOR `\\"`
void parse(const char *filename, Pitt_token *token_list)
{
    ifstream file{filename};
    string tokens{};
    string temp_buffer{};

    int tc_count = -1;
    int col_ctr = {};
    int line = {};

    while (getline(file, temp_buffer))
    {
        line++;
        int waitfor = 0;
        bool encountered = false;

        istringstream serve(temp_buffer);
        while (serve >> tokens)
        {
            col_ctr = static_cast<int>(temp_buffer.find(tokens));

            if (waitfor > 0 && encountered == true)
            {
                waitfor--;
                if (waitfor == 0)
                    encountered = false;
            }

            if (tokens[0] == '"' && encountered == false)
            {
                encountered = true;
                bool unescaped = false;
                string lexedstr = "";
                int scopeCount = 0;

                size_t begin = static_cast<int>(col_ctr);
                size_t end = begin + 1;

                for (size_t dupend = end; dupend < temp_buffer.size(); dupend++)
                {
                    unescaped = true;
                    if (temp_buffer[dupend] == '"' && temp_buffer[dupend - 1] != '\\')
                    {
                        if (temp_buffer[dupend + 1] == ' ' || temp_buffer[dupend + 1] == '\n' || temp_buffer[dupend + 1] == '\0')
                        {
                            unescaped = false;
                            end = dupend;
                            break;
                        }
                    }
                }

                for (size_t p = begin; p <= end; p++)
                {
                    if (temp_buffer[p] == '"' && temp_buffer[p - 1] != '\\')
                    {
                        scopeCount++;
                    }
                }

                string word_finder = "";

                for (size_t k = begin; k <= end; k++)
                    word_finder += temp_buffer[k];

                for (size_t k = begin + 1; k < end; k++)
                    lexedstr += temp_buffer[k];

                waitfor = parse_words_in_text(word_finder);

                if (unescaped == false && scopeCount == 2)
                {
                    tc_count++;
                    token_list[tc_count].token = lexedstr;
                    token_list[tc_count].r = line;
                    token_list[tc_count].c = col_ctr;
                    token_list[tc_count].type = P_STR;
                }
                else
                {
                    tc_count++;
                    token_list[tc_count].token = lexedstr;
                    token_list[tc_count].r = line;
                    token_list[tc_count].c = col_ctr;
                    token_list[tc_count].type = P_STR;
                    token_list[tc_count].unescaped = true;
                }
            }

            else if (waitfor == 0)
            {
                tc_count++;
                token_list[tc_count].token = tokens;
                token_list[tc_count].r = line;
                token_list[tc_count].c = col_ctr;

                if (isnum(tokens))
                {
                    token_list[tc_count].type = P_INT;
                }
                else
                {
                    token_list[tc_count].type = fetch_type(tokens);
                }
            }
        }
    }

    /*
    while (getline(file, temp_buffer))
    {
        line++;


        int begin = {}, end = {} , ignore_words = {};
        bool pass1 = false;
        string lexstr = {};
        begin = static_cast<int>(temp_buffer.find('"'));

        if (begin != -1)
        {
            for (end = begin + 1; end < temp_buffer.size(); end++)
            {
                if (temp_buffer[end] == '"' && temp_buffer[end-1] != '\\'){
                    pass1 = true;
                    lexstr = temp_buffer.substr(begin+1,end-1);
                    ignore_words = parse_words_in_text(lexstr);
                    break;
                }
            }
        }

        istringstream pipe(temp_buffer);
        while (pipe >> tokens)
        {
            col_ctr = static_cast<int>(temp_buffer.find(tokens));

            if(pass1 == true && (col_ctr >= begin && col_ctr < end)){
                tc_count++;
                token_list[tc_count].token = lexstr;
                token_list[tc_count].r = line;
                token_list[tc_count].c = begin;
                token_list[tc_count].type = P_STR;
                pass1 = false;
            }

            else if(ignore_words > 0 && (col_ctr >= begin && col_ctr < end)){
                ignore_words--;
            }

            if(ignore_words == 0 || col_ctr < begin || col_ctr > end){
                tc_count++;
                token_list[tc_count].token = tokens;
                token_list[tc_count].r = line;
                token_list[tc_count].c = col_ctr;

                if (isnum(tokens))
                {
                    token_list[tc_count].type = P_INT;
                }
                else
                {
                    token_list[tc_count].type = fetch_type(tokens);
                }
            }
        }
    }
    */

    file.close();
}

int EXPECT_THEN(int current, Pitt_token *list, const size_t &size)
{
    int location = -1;
    current += 1;
    for (; current < static_cast<int>(size); current++)
    {
        if (list[current].type == P_IF)
        {
            break;
        }
        else if (list[current].type == P_WHILE)
        {
            break;
        }
        else if (list[current].type == P_END)
        {
            break;
        }
        else if (list[current].type == P_THEN)
        {
            location = current;
            break;
        }
    }
    return location;
}

int EXPECT_END(int current, Pitt_token *list, const size_t &size, stack<PittType> &ptok)
{
    int location = -1;
    current++;

    int delay = 0;
    for (; current < static_cast<int>(size); current++)
    {
        if (list[current].type == P_IF || list[current].type == P_WHILE)
        {
            delay++;
        }
        else if (list[current].type == P_END)
        {
            if (delay == 0)
            {
                location = current;
                break;
            }
            else
            {
                delay--;
            }
        }
        if (ptok.top() == P_IF)
        {
            if (list[current].type == P_ELIF)
            {
                if (delay == 0)
                {
                    location = current;
                    break;
                }
            }
            else if (list[current].type == P_ELSE)
            {
                if (delay == 0)
                {
                    location = current;
                    break;
                }
            }
        }
    }
    return location;
}

int EXPECT_ONLY_END(int current, Pitt_token *list, const size_t &size)
{
    int location = -1;
    current++;

    int delay = 0;
    for (; current < static_cast<int>(size); current++)
    {
        if (list[current].type == P_IF || list[current].type == P_WHILE)
        {
            if (delay == 0)
            {
                delay++;
            }
        }
        else if (list[current].type == P_END)
        {
            if (delay == 0)
            {
                location = current;
                break;
            }
            else
            {
                delay--;
            }
        }
    }
    return location;
}

#define MEM_CAP 140000

void simulate_file(const char *filename, Pitt_token *token_list)
{
    stack<int> numeric_stack = {};
    const size_t size = words_count(filename);

    // Comment Handling Variables
    bool isComment = false;
    int PreviousLine = 0;

    // If/While handling
    int CondEncounter = {};
    stack<PittType> ptoken;
    stack<int> wloc;
    stack<bool> wcon;

    // Memory Handling
    byte memory[MEM_CAP] = {};
    size_t free_ptr = 0;

    int j = {};
    while (j < size)
    {
        if (token_list[j].type == P_COMMENT)
        {
            PreviousLine = token_list[j].r;
            isComment = true;
        }

        else if (isComment == true && (token_list[j].r > PreviousLine))
        {
            isComment = false;
            PreviousLine = 0;
        }

        if (isComment == false && token_list[j].type != P_EOT)
        {
            if (token_list[j].type == P_INT)
            {
                numeric_stack.push(stoi(token_list[j].token));
            }

            if (token_list[j].type == P_STR)
            {
                if (token_list[j].unescaped == true)
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unescaped string literal found" << endl;
                    break;
                }

                string &raw = token_list[j].token;
                bool last_esc = false;
                size_t pusher = free_ptr;

                size_t j = 0;
                while (j < raw.size())
                {
                    if (raw[j] == '\\')
                    {
                        j++;
                        if (raw[j] == 'n')
                        {
                            memory[pusher] = '\n';
                            pusher++;
                        }
                        else if (raw[j] == 'b')
                        {
                            pusher--;
                            memory[pusher] = '\b';
                            pusher++;
                        }
                        else if (raw[j] == 't')
                        {
                            memory[pusher] = '\t';
                            pusher++;
                        }
                        else if (raw[j] == '0')
                        {
                            memory[pusher] = '\0';
                            pusher++;
                        }
                        else if (raw[j] == '"')
                        {
                            memory[pusher] = '"';
                            pusher++;
                        }
                        else if (raw[j] == '\\')
                        {
                            memory[pusher] = '\\';
                            pusher++;
                        }
                        else if (raw[j] == 'r')
                        {
                            memory[pusher] = '\r';
                            pusher++;
                        }
                        else if (raw[j] == 'v')
                        {
                            memory[pusher] = '\v';
                            pusher++;
                        }
                    }
                    else
                    {
                        memory[pusher] = raw[j];
                        pusher++;
                    }
                    j++;
                }

                numeric_stack.push(free_ptr);
                free_ptr += pusher;
                numeric_stack.push(free_ptr);
            }

            if (token_list[j].type == P_PLUS)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b + a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `PLUS` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_MIN)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b - a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `MINUS` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_MUL)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b * a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `MUL` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_DIV)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b / a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `DIV` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BOR)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b | a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `BOR` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BAND)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b & a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `BAND` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BNOT)
            {
                if (numeric_stack.size() > 0)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(!a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `NOT` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BSHL)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b << a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `SHL` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BSHR)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b >> a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `SHR` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_BXOR)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b ^ a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `XOR` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_NE)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b != a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `!=` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_E)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b == a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `=` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_L)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b < a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `<` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_G)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b > a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `>` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_GEQ)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b >= a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `>=` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_LEQ)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b <= a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `<=` operator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_DUP)
            {
                if (numeric_stack.size() > 0)
                {
                    numeric_stack.push(numeric_stack.top());
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `DUP` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_SWAP)
            {
                if (numeric_stack.size() > 1)
                {
                    float a = numeric_stack.top();
                    numeric_stack.pop();
                    float b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(a);
                    numeric_stack.push(b);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `SWAP` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_DROP)
            {
                if (numeric_stack.size() > 0)
                {
                    numeric_stack.pop();
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `DROP` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_OVER)
            {
                if (numeric_stack.size() > 1)
                {
                    float a = numeric_stack.top();
                    numeric_stack.pop();
                    float b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b);
                    numeric_stack.push(a);
                    numeric_stack.push(b);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `OVER` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_ROT)
            {
                if (numeric_stack.size() > 2)
                {
                    float a = numeric_stack.top();
                    numeric_stack.pop();
                    float b = numeric_stack.top();
                    numeric_stack.pop();
                    float c = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(a);
                    numeric_stack.push(c);
                    numeric_stack.push(b);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `ROT` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_NROT)
            {
                if (numeric_stack.size() > 2)
                {
                    float c = numeric_stack.top();
                    numeric_stack.pop();
                    float b = numeric_stack.top();
                    numeric_stack.pop();
                    float a = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(c);
                    numeric_stack.push(a);
                    numeric_stack.push(b);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `-ROT` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_NIP)
            {
                if (numeric_stack.size() > 1)
                {
                    int b = numeric_stack.top();
                    numeric_stack.pop();
                    numeric_stack.pop();
                    numeric_stack.push(b);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `NIP` manipulator" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_PER)
            {
                if (numeric_stack.size() > 1)
                {
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b % a);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments provided to `MODULO` OPERATOR" << endl;
                    break;
                }
            }

            // TODO: Underconstruction
            if (token_list[j].type == P_ADDR8)
            {
                numeric_stack.push(static_cast<int>(free_ptr));
            }

            if (token_list[j].type == P_DEREF8)
            {
                if (!numeric_stack.empty())
                {
                    int loc = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(static_cast<int>(memory[loc]));
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: no `PTR` provided to dereference from `I8` memory" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_PUSH8)
            {
                if (numeric_stack.size() > 1)
                {
                    int data = numeric_stack.top();
                    numeric_stack.pop();
                    int loc = numeric_stack.top();
                    numeric_stack.pop();

                    memory[loc] = data & 0xFF;
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected a `PTR` and `DATA` to store to `I8` memory" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_FUNC_PUTS)
            {
                if (numeric_stack.size() > 1)
                {
                    int end = numeric_stack.top();
                    numeric_stack.pop();
                    int begin = numeric_stack.top();
                    numeric_stack.pop();

                    for (size_t j = static_cast<size_t>(begin); j < static_cast<size_t>(end); j++)
                        printf("%c", memory[j]);
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments to function `[int: begin int: end] puts`" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_FUNC_PUTC)
            {
                if (numeric_stack.size() > 0)
                {
                    printf("%c", memory[numeric_stack.top()]);
                    numeric_stack.pop();
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments to function `[int: loc] putc`" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_FUNC_WRITE)
            {
                if (numeric_stack.size() > 3)
                {
                    int syscallnum = numeric_stack.top();
                    numeric_stack.pop();

                    int arg1 = numeric_stack.top();
                    numeric_stack.pop();

                    int arg2 = numeric_stack.top();
                    numeric_stack.pop();

                    int arg3 = numeric_stack.top();
                    numeric_stack.pop();

                    if (syscallnum == 1)
                    {
                        if (arg1 == 1)
                        {
                            for (int p = arg2; p < arg2 + arg3; p++)
                                cout << memory[p];
                        }
                        else if (arg1 == 2)
                        {
                            for (int p = arg2; p < arg2 + arg3; p++)
                                cerr << memory[p];
                        }
                        else
                        {
                            cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unknown FileDescriptor provided  " << arg1 << " to function [int: arg3 int: arg2 int: arg1 int: syscall] write" << endl;
                            break;
                        }
                    }
                    else
                    {
                        cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unknown syscall number provided  " << syscallnum << " to function [int: arg3 int: arg2 int: arg1 int: syscall] write" << endl;
                        break;
                    }
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: too few arguments to function [int: arg3 int: arg2 int: arg1 int: syscall] write" << endl;
                    break;
                }
            }
            // TODO: Underconstruction

            if (token_list[j].type == P_IF)
            {
                int location = EXPECT_THEN(j, token_list, size);
                ptoken.push(P_IF);

                if (location == -1)
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: incomplete `IF` statement definition expected a `then`" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_THEN)
            {
                if (!ptoken.empty() && (ptoken.top() == P_IF || ptoken.top() == P_WHILE))
                {
                    if (ptoken.top() == P_WHILE)
                    {
                        wcon.push(static_cast<bool>(numeric_stack.top()));
                    }
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `then` keyword" << endl;
                    break;
                }

                int jmp = EXPECT_END(j, token_list, size, ptoken);
                if (jmp == -1)
                {
                    if (ptoken.top() == P_IF)
                    {
                        cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected an `END` of this `if` statement" << endl;
                    }
                    else if (ptoken.top() == P_WHILE)
                    {
                        cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected an `END` of this `while` statement" << endl;
                    }
                    break;
                }

                CondEncounter = numeric_stack.top();
                numeric_stack.pop();

                if (CondEncounter == true)
                {
                }
                else if (CondEncounter == false)
                {
                    j = jmp;
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected a `BOOL` but found `INT`" << endl;
                    break;
                }
            }
            if (token_list[j].type == P_ELIF)
            {
                if (!ptoken.empty() && ptoken.top() == P_IF)
                {
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `elif` keyword" << endl;
                    break;
                }

                int jmp = EXPECT_END(j, token_list, size, ptoken);
                int location = EXPECT_THEN(j, token_list, size);

                if (location == -1)
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: incomplete `ELIF` statement definition expected a `then`" << endl;
                    break;
                }

                if (jmp == -1)
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected an `END` of this `elif` statement" << endl;
                    break;
                }

                if (CondEncounter == false)
                {
                    CondEncounter = true;
                }
                else if (CondEncounter == true)
                {
                    j = EXPECT_ONLY_END(j, token_list, size);
                }
            }
            if (token_list[j].type == P_ELSE)
            {
                if (!ptoken.empty() && ptoken.top() == P_IF)
                {
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `else` keyword" << endl;
                    break;
                }

                if (CondEncounter == false)
                {
                    CondEncounter = true;
                }
                else if (CondEncounter == true)
                {
                    j = EXPECT_ONLY_END(j, token_list, size);
                }
            }

            if (token_list[j].type == P_END)
            {
                if (!ptoken.empty() && (ptoken.top() == P_IF || ptoken.top() == P_WHILE))
                {
                    if (ptoken.top() == P_WHILE)
                    {
                        if (!wcon.empty() && !wloc.empty())
                        {
                            if (wcon.top() == true)
                            {
                                j = wloc.top();
                            }
                            wcon.pop();
                            wloc.pop();
                        }
                    }
                    ptoken.pop();
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `end` keyword" << endl;
                    break;
                }
            }

            if (token_list[j].type == P_WHILE)
            {
                int location = EXPECT_THEN(j, token_list, size);

                if (location == -1)
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: incomplete `WHILE` statement definition expected a `then`" << endl;
                    break;
                }

                ptoken.push(P_WHILE);
                wloc.push(j);
            }

            if (token_list[j].type == P_FUNC_PRINT)
            {
                if (!numeric_stack.empty())
                {
                    cout << numeric_stack.top() << endl;
                    numeric_stack.pop();
                }
                else
                {
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: Too few arguments to function `[int: data] print`" << endl;
                    break;
                }
            }
            if (token_list[j].type == P_UNDEF)
            {
                cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: invalid word found `" << token_list[j].token << "`" << endl;
                exit(1);
            }
        }
        j += 1;
    }
    if (numeric_stack.size() > 0)
    {
        cout << filename << ":" << token_list[j - 1].r << ":" << token_list[j - 1].c << ": ERROR: unhandled data on stack" << endl;
        exit(0);
    }
}
void dump_file(const char *filename, const Pitt_token *token_list)
{
    printf("[INFO] Reading: `%s`\n\n", filename);
    for (size_t j = 0; j < words_count(filename); j++)
    {
        cout << "<name:" << token_list[j].token << ",type:" << token_list[j].type << ",line:" << token_list[j].r << ",addr:" << &token_list[j] << ">" << endl;
    }
}

int main(int args, char **argv)
{
    [args, argv]()
    {
        if (args < 3)
        {
            printf("pitt:\033[0;31m fatal error:\033[0;37m no input files or operation type provided!\n");
            help();
        }
        else
        {
            init_map();
            // Filename :- argv[2]
            // Command :- argv[1]
            check_file_ext(argv[2]);
            check_file_val(argv[2]);

            if (strcmp(argv[1], "run") == 0)
            {
                printf("[INFO] Simulating file: `%s`\n\n", argv[2]);
                Pitt_token *tokens = new Pitt_token[words_count(argv[2])];
                for (size_t j = 0; j < words_count(argv[2]); j++)
                    tokens[j].type = P_EOT;
                parse(argv[2], tokens);
                simulate_file(argv[2], tokens);
            }
            else if (strcmp(argv[1], "dump") == 0)
            {
                Pitt_token *tokens = new Pitt_token[words_count(argv[2])];
                parse(argv[2], tokens);
                dump_file(argv[2], tokens);
            }
            else
            {
                printf("pitt:\033[0;31m fatal error:\033[0;37m invalid operation type provided `%s`!\n", argv[1]);
                help();
            }
        }
    }();
    return 0;
}