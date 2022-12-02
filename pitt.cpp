// TODO LIST:
// TODO: Add `while` statements
// TODO: Add support for Byte Memory for holding characters
// TODO: Add strings
// TODO: Add different value stack-pointers (@i8,@i16,@i32,@i64)
// TODO: Add float support [Not as main] (@f32,@f64)
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

int words_count(const char* filename)
{   
    int _count = 0;
    fstream file;
    file.open(filename,ios_base::in);
    
    string temp {};
    while(file >> temp)
        _count++;
    file.close();
    return _count;
}  

void check_file_ext(const char* filename)
{
    size_t j {};
    for(;j < strlen(filename); j++){
        if(filename[j] == '.' && filename[j+1] == '/'){
            
        }else if(filename[j] == '.'){
            break;
        }
    }
    char buffer[strlen(filename)-(j+1)] = {'\0'};
    uint8_t buf_c = 0;
    j++;
    for(;j < strlen(filename); j++){
        buffer[buf_c++] = filename[j];
    }
    buffer[buf_c++] = '\0';
    if(strcmp(buffer,PITT_EXT) != 0){
        printf("pitt: \033[0;31mfatal error:\033[0;37m invalid file format provided `%s`\n\n",filename);      
        exit(1);
    }
}

void check_file_val(const char* filename)
{
    fstream file;
    file.open(filename,ios_base::in);

    if(file.is_open()){
        file.close();
    }else{
        printf("forth:\033[0;31m fatal error:\033[0;37m can not open file to read `%s`\n\n",filename);
        exit(1);
    }
}

bool isnum(string& text)
{
    bool state = false;
    size_t _c = {};
    for(size_t i = 0; i < text.size(); i++){
        if(i == 0 && (text[i] == '-' || text[i] == '+') && text.size() > 1){
            _c++;
        }
        else if(text[i] >= '0' && text[i] <= '9'){
            _c++;
        }
    }

    if(_c == text.size()){
        state = true;
    }
    
    return state;
}
bool isflt(string& text)
{
    bool state = false;
    size_t _c = {};
    for(size_t i = 0; i < text.size(); i++){
        if(i == 0 && (text[i] == '-' || text[i] == '+') && text.size() > 1){
            _c++;
        }
        else if(text[i] >= '0' && text[i] <= '9'){
            _c++;
        }else if(text[i] == '.' && (text[i+1] >= '0' && text[i+1] <= '9')){
            _c++;
        }
    }

    if(_c == text.size()){
        state = true;
    }
    
    return state;
}
PittType fetch_type(string& text)
{
    PittType rettype = P_UNDEF;
    for(auto it : usable_map)
    {
        if(text == it.first){
            rettype = it.second;
            break;
        }
    }
    return rettype;
}

// To Avoid memory leak we are taking *token_list from user
void parse(const char* filename,Pitt_token *token_list)
{
    fstream file;
    file.open(filename);
    string tokens{};
    string temp_buffer{};

    int tc_count = 0;
    int col_ctr = {};
    int line = {};

    while(getline(file,temp_buffer)){
        istringstream pipe(temp_buffer);
        line++;
        while(pipe >> tokens){
            col_ctr = static_cast<int>(temp_buffer.find(tokens));

            token_list[tc_count].token = tokens;
            token_list[tc_count].r = line;
            token_list[tc_count].c = col_ctr;
                
            if(isnum(tokens)){
                token_list[tc_count].type = P_INT;
            }
            else{
                token_list[tc_count].type = fetch_type(tokens);
            }
            tc_count++;
        }
    }
    file.close();
}

int EXPECT_THEN(int current,Pitt_token* list,const size_t& size)
{
    int location = -1;
    current += 1;
    for(;current < static_cast<int>(size); current++){
        if(list[current].type == P_IF){
            break;
        }
        else if(list[current].type == P_WHILE){
            break;
        }
        else if(list[current].type == P_END){
            break;
        }
        else if(list[current].type == P_THEN){
            location = current;
            break;
        }
    }
    return location;
}

int EXPECT_END(int current,Pitt_token *list,const size_t& size)
{
    int location = -1;
    current++;

    int delay = 0;
    for(;current < static_cast<int>(size); current++){
        if(list[current].type == P_IF || list[current].type == P_WHILE){
            if(delay == 0){
                delay++;
            }
        }
        else if(list[current].type == P_END){
            if(delay == 0){
                location = current;
                break;
            }
            else{
                delay--;
            }
        }
        else if(list[current].type == P_ELIF){
            if(delay == 0){
                location = current;
                break;
            }
        }
        else if(list[current].type == P_ELSE){
            if(delay == 0){
                location = current;
                break;
            }
        }
    }
    return location;
}

int EXPECT_ONLY_END(int current,Pitt_token *list,const size_t& size)
{
    int location = -1;
    current++;

    int delay = 0;
    for(;current < static_cast<int>(size); current++){
        if(list[current].type == P_IF || list[current].type == P_WHILE){
            if(delay == 0){
                delay++;
            }
        }
        else if(list[current].type == P_END){
            if(delay == 0){
                location = current;
                break;
            }
            else{
                delay--;
            }
        }
    }
    return location;
}


void simulate_file(const char* filename,Pitt_token* token_list)
{
    stack<int> numeric_stack = {};
    const size_t size = words_count(filename); 

    // Comment Handling Variables
    bool isComment = false;
    int PreviousLine = 0;

    // If/While handling
    int CondEncounter = {};
    stack<PittType> ptoken;

    for(size_t j = 0; j < size; j++)
    {
        if(token_list[j].type == P_COMMENT)
        {
            PreviousLine = token_list[j].r;
            isComment = true;
        }

        else if(isComment == true && (token_list[j].r > PreviousLine)){
            isComment = false;
            PreviousLine = 0;
        }      

        if(isComment == false){
            if(token_list[j].type == P_INT){
                numeric_stack.push(stoi(token_list[j].token));
            }

            if(token_list[j].type == P_PLUS){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b+a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `+` instead found less" << endl;
                }
            }
            
            if(token_list[j].type == P_MIN){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b-a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `-` instead found less" << endl;
                }
            }
            
            if(token_list[j].type == P_MUL){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b*a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `*` instead found less" << endl;
                }
            }
            
            if(token_list[j].type == P_DIV){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b/a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `/` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_BOR){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b | a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `bor` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_BAND){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b & a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `band` instead found less" << endl;
                }
            }
            
            if(token_list[j].type == P_BNOT){
                if(numeric_stack.size() > 0){
                    int a = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(!a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 1 literals to perform the operation `bnot` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_BSHL){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b << a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `bshl` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_BSHR){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b >> a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `bshr` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_BXOR){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b ^ a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `bxor` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_NE){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b != a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `!=` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_E){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b == a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `=` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_L){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b < a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `<` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_G){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b > a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `>` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_GEQ){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b >= a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `>=` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_LEQ){
                if(numeric_stack.size() > 1){
                    int a = numeric_stack.top();
                    numeric_stack.pop();
                    int b = numeric_stack.top();
                    numeric_stack.pop();

                    numeric_stack.push(b <= a);
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 2 literals to perform the operation `<=` instead found less" << endl;
                }
            }

            if(token_list[j].type == P_DUP){
                numeric_stack.push(numeric_stack.top());
            }
            
            if(token_list[j].type == P_SWAP){
                float a = numeric_stack.top();
                numeric_stack.pop();
                float b = numeric_stack.top();
                numeric_stack.pop();

                numeric_stack.push(a);
                numeric_stack.push(b);
            }
            
            if(token_list[j].type == P_DROP){
                numeric_stack.pop();
            }
        
            if(token_list[j].type == P_OVER)
            {
                float a = numeric_stack.top();
                numeric_stack.pop();
                float b = numeric_stack.top();
                numeric_stack.pop();
        
                numeric_stack.push(b);
                numeric_stack.push(a);
                numeric_stack.push(b);
            }
          
            if(token_list[j].type == P_ROT)
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

            if(token_list[j].type == P_NROT)
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

            if(token_list[j].type == P_NIP)
            {
                int b = numeric_stack.top();
                numeric_stack.pop();
                numeric_stack.pop();
                numeric_stack.push(b);
            }

            if(token_list[j].type == P_IF){
                int location = EXPECT_THEN(j,token_list,size);
                ptoken.push(P_IF);

                if(location == -1){
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: incomplete `if` statement definition" << endl;   
                    break;
                }          
            }

            if(token_list[j].type == P_THEN){
                if(!ptoken.empty() && (ptoken.top() == P_IF || ptoken.top() == P_WHILE)){
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `then` keyword" << endl;   
                    break;
                }

                int jmp = EXPECT_END(j,token_list,size);
                if(jmp == -1){
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected an `end` if THIS `if` statement" << endl;
                    break;
                } 

                CondEncounter = numeric_stack.top();
                numeric_stack.pop();

                if(CondEncounter == true){

                }
                else if(CondEncounter == false){
                    j = jmp;
                } 
                else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected a `BOOL` but found something else" << endl;
                    break;
                }
            }
            if(token_list[j].type == P_ELIF){
                if(!ptoken.empty() && ptoken.top() == P_IF){
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `elif` keyword" << endl;   
                    break;
                }

                int jmp = EXPECT_END(j,token_list,size);
                int location = EXPECT_THEN(j,token_list,size);
                
                if(location == -1){
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: incomplete `elif` statement definition" << endl;   
                    break;
                }          

                if(jmp == -1){
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: expected an `end` if THIS `elif` statement" << endl;
                    break;
                } 

                if(CondEncounter == false){
                    CondEncounter = true;
                }
                else if(CondEncounter == true){
                    j = EXPECT_ONLY_END(j,token_list,size);
                }
            }
            if(token_list[j].type == P_ELSE){
                if(!ptoken.empty() && ptoken.top() == P_IF){
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `else` keyword" << endl;   
                    break;
                }

                if(CondEncounter == false){
                    CondEncounter = true;
                }
                else if(CondEncounter == true){
                    j = EXPECT_ONLY_END(j,token_list,size);
                }
            }
            if(token_list[j].type == P_END)
            {
                if(!ptoken.empty() && (ptoken.top() == P_IF || ptoken.top() == P_WHILE)){
                    ptoken.pop();
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: unexpected occurence of `end` keyword" << endl;   
                    break; 
                }
            }

            if(token_list[j].type == P_FUNC_PRINT){
                if(!numeric_stack.empty()){
                    cout << numeric_stack.top() << endl;
                    numeric_stack.pop();
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: Too few arguments to function `print`" << endl;
                    break;
                }
            }
            if(token_list[j].type == P_UNDEF)
            {
                cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: invalid word found `" << token_list[j].token << "`" << endl << endl;
                exit(1);
            }
        }
    }
}
void dump_file(const char* filename,const Pitt_token* token_list)
{
    printf("[INFO] Reading: `%s`\n\n",filename);
    for(size_t j = 0; j < words_count(filename); j++){
        cout<<"<name:"<<token_list[j].token<<",type:"<< token_list[j].type << ",line:"<<token_list[j].r << 
        ",addr:" << &token_list[j] << ">" << endl;
    }
}



int main(int args,char **argv)
{
    [args,argv](){
        if(args < 3){
            printf("pitt:\033[0;31m fatal error:\033[0;37m no input files or operation type provided!\n");
            help();
        }else{
            init_map();
            // Filename :- argv[2]
            // Command :- argv[1]
            check_file_ext(argv[2]);
            check_file_val(argv[2]);

            if(strcmp(argv[1],"run") == 0){
                printf("[INFO] Simulating file: `%s`\n\n",argv[2]);
                Pitt_token *tokens = new Pitt_token[words_count(argv[2])];
                parse(argv[2],tokens);
                simulate_file(argv[2],tokens);
            }
            else if(strcmp(argv[1],"dump") == 0){
                Pitt_token *tokens = new Pitt_token[words_count(argv[2])];
                parse(argv[2],tokens);
                dump_file(argv[2],tokens);
            }
            else{
                printf("pitt:\033[0;31m fatal error:\033[0;37m invalid operation type provided `%s`!\n",argv[1]);
                help();
            }
        }
    }();
    return 0;
}