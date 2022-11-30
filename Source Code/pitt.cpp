// TODO: Add `if` , `while` statement
// TODO: Add memory/character/string
// TODO: Optimize code
// TODO: Add support for `floats` but not as main operation stack

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

    if(strcmp(buffer,PITT_EXT) == 0){

    }else{
        printf("pitt: \033[0;31mfatal error:\033[0;37m invalid file format provided `%s`\n",filename);
        printf("\033[0;31msimulation terminated\033[0;37m");        
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
        printf("forth:\033[0;31m fatal error:\033[0;37m can not open file to read `%s`\n",filename);
        printf("\033[0;31msimulation terminated\033[0;37m");
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

void simulate_file(const char* filename,Pitt_token* token_list)
{
    stack<int> numeric_stack = {};
    size_t size = words_count(filename);

    // Comment Handling Variables
    bool isComment = false;
    int PreviousLine = 0;

    for(size_t j = 0; j < size; j++)
    {
        if(token_list[j].type == P_COMMENT)
        {
            PreviousLine = token_list[j].r;
            isComment = true;
        }

        else if(isComment == true && (token_list[j].r == PreviousLine+1)){
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


            if(token_list[j].type == P_FUNC_PRINT){
                if(!numeric_stack.empty()){
                    cout << numeric_stack.top() << endl;
                    numeric_stack.pop();
                }else{
                    cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": WARNING: expected 1 stack-valid argument for `print` function found none" << endl;
                }
            }
            if(token_list[j].type == P_UNDEF)
            {
                cout << filename << ":" << token_list[j].r << ":" << token_list[j].c << ": ERROR: invalid token found `" << token_list[j].token << "`" << endl;
                printf("operation terminated with exit code -1\n");
                exit(-1);
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