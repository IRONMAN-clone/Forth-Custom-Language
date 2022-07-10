// Note: before comments were parsed but now comments are not even parsed to avoid some errors

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define MEMORY_SIZE 6000 // # 6KB should be enough for everyone
#define STRING_CAPACITY 6000 // #6KB for string-memory capacity would be enough for everyone

bool extension_check(const char* program)
{
    int break1 = 0;
    bool ext_pass = false;
    
    for(int i = 0; program[i] != '\0'; i++){
	if(program[i] == '.'){
	    break1 = i;
	    break;
	}
    }

    char *buffer = (char*)malloc(sizeof(char)*10);
    int n = 0;
    
    for(int j = break1+1;program[j] != '\0'; j++){
	buffer[n] = program[j];
	n++;
    }
    buffer[n++] = '\0';
    // printf("FOUND: %s\n",buffer);
    if(strcmp(buffer,"pearl") == 0){
	ext_pass = true;
    }
    return ext_pass;
}

void usage()
{
    printf("Usage: ./gpearl [SUBCOMMAND] [ARGS] [OPTS]\n");
    printf("Subcommands:\n");
    printf("   -s    simulates the file\n");
    printf("   -l    produces lexical analysis of the file\n");     
    printf("Args:\n");
    printf("   sim     expects: <base-file>\n");
    printf("   lex     expects: <base-file>\n");
    printf("Options:\n");
    printf("   -t      shows the time taken to produce output\n");
}


typedef enum {
    // datatypes
    INT,
    STR,
    CSTR,
    CHAR,

    // operators
    ADD,
    MIN,
    MUL,
    DIV,
    EQ,
    NEQ,
    GT,
    LT,
    GE,
    LE,
    NOT,
    BAND,
    BOR,
    BEOR,
    SHL,
    SHR,
    
    // functions
    PRINT,
    WRITE,
    PUTS,
    EXIT,
    STRCMP,
    STRLEN,
    
    // keywords
    DUP,
    MOD,
    SWAP,
    DROP,
    OVER,
    ROT,
    NIP,
    DUPD,
    SWAPD,
    MEMORY,
    MEMREAD,
    MEMPTR,

    // blocks
    IF,
    THEN,
    END,
    ELSE,
    WHILE,

    // unexpected
    COMMENT,
    UNEXPECTED
}TokenType;
char *tt_sign[] = {"int","str","cstr","char","+","-","*","/","=","!=",">","<",">=","<=","not","band","bor","beor","shl","shr","print","write","puts","exit","strcmp","lenof","dup","mod","swap","drop","over","rot","nip","dupd","swapd","memory","memread","*memory","if","then","end","else","while",";;","unexpected"};


typedef struct lexemes{
    int line;
    char *raw;
    TokenType tt_type;
}Token;

bool is_int(char *token)
{
    int expect = strlen(token);
    int found = 0;
    bool status = false;

    if(token[0] == '-'){
        if(strlen(token) > 1){
	    found++;
	}
    }

    for(int i = 0; i < strlen(token); i++){
	if(token[i] >= '0' && token[i] <= '9'){
	    found++;
	}
    }

    if(found == expect){
	status = true;
    }
    return status;
}

// String-Functions
bool starts_with(char *string,char target)
{
    bool found = false;

    if(string[0] == string[1] && string[0] == target){
	found = true;
    }
    return found;
}

TokenType check_token_type(char *token)
{
    int type = UNEXPECTED; 
    if(is_int(token) == true){
	type = INT;
    }
    else if(strcmp(token,tt_sign[ADD]) == 0){
	type = ADD;
    }
    else if(strcmp(token,tt_sign[MIN]) == 0){
	type = MIN;
    }
    else if(strcmp(token,tt_sign[MUL]) == 0){
	type = MUL;
    }
    else if(strcmp(token,tt_sign[DIV]) == 0){
	type = DIV;
    }
    else if(strcmp(token,tt_sign[PRINT]) == 0){
	type = PRINT;
    }
    else if(strcmp(token,tt_sign[DUP]) == 0){
	type = DUP;
    }
    else if(strcmp(token,tt_sign[MOD]) == 0){
	type = MOD;
    }
    else if(strcmp(token,tt_sign[SWAP]) == 0){
	type = SWAP;
    }
    else if(strcmp(token,tt_sign[DROP]) == 0){
	type = DROP;
    }
    else if(strcmp(token,tt_sign[OVER]) == 0){
	type = OVER;
    }
    else if(strcmp(token,tt_sign[ROT]) == 0){
	type = ROT;
    }
    else if(strcmp(token,tt_sign[NIP]) == 0){
	type = NIP;
    }
    else if(strcmp(token,tt_sign[DUPD]) == 0){
	type = DUPD;
    }
    else if(strcmp(token,tt_sign[SWAPD]) == 0){
	type = SWAPD;
    }
    else if(strcmp(token,tt_sign[EQ]) == 0){
	type = EQ;
    }
    else if(strcmp(token,tt_sign[NEQ]) == 0){
	type = NEQ;
    }
    else if(strcmp(token,tt_sign[GT]) == 0){
	type = GT;
    }
    else if(strcmp(token,tt_sign[LT]) == 0){
	type = LT;
    }
    else if(strcmp(token,tt_sign[GE]) == 0){
	type = GE;
    }
    else if(strcmp(token,tt_sign[LE]) == 0){
	type = LE;
    }
    // here
     else if(strcmp(token,tt_sign[NOT]) == 0){
	type = NOT;
    }
    else if(strcmp(token,tt_sign[BAND]) == 0){
	type = BAND;
    }
    else if(strcmp(token,tt_sign[BOR]) == 0){
	type = BOR;
    }
    else if(strcmp(token,tt_sign[BEOR]) == 0){
	type = BEOR;
    }
    else if(strcmp(token,tt_sign[SHL]) == 0){
	type = SHL;
    }
    else if(strcmp(token,tt_sign[SHR]) == 0){
	type = SHR;
    }
    else if(strcmp(token,tt_sign[IF]) == 0){
	type = IF;
    }
    else if(strcmp(token,tt_sign[THEN]) == 0){
	type = THEN;
    }
    else if(strcmp(token,tt_sign[END]) == 0){
	type = END;
    }
    /*
    else if((strcmp(token,tt_sign[COMMENT]) == 0) || (starts_with(token,';') == true)){
      type = COMMENT;
    }
    */
    else if(strcmp(token,tt_sign[ELSE]) == 0){
	type = ELSE;
    }
    else if(strcmp(token,tt_sign[WHILE]) == 0){
	type = WHILE;
    }
    else if(strcmp(token,tt_sign[MEMORY]) == 0){
	type = MEMORY;
    }
    else if(strcmp(token,tt_sign[MEMREAD]) == 0){
	type = MEMREAD;
    }
    else if(strcmp(token,tt_sign[MEMPTR]) == 0){
	type = MEMPTR;
    }
    else if(strcmp(token,tt_sign[WRITE]) == 0){
	type = WRITE;
    }
    else if(strcmp(token,tt_sign[PUTS]) == 0){
	type = PUTS;
    }
    else if(strcmp(token,tt_sign[EXIT]) == 0){
	type = EXIT;
    }
    else if(strcmp(token,tt_sign[STRCMP]) == 0){
	type = STRCMP;
    }
    else if(strcmp(token,tt_sign[STRLEN]) == 0){
	type = STRLEN;
    }
    
    return type;
}

int file_word_count(const char* program)
{
    FILE *pipe = fopen(program,"r");

    char buffer[256];
    int words = 0;

    char chop[] = {' ','\n','\0'};
    
    while(fgets(buffer,256,pipe) != NULL){
	char *tokenize = strtok(buffer,chop);
	while(tokenize != NULL){
	    words++;
	    tokenize = strtok(NULL,chop);
	}
    }
    fclose(pipe);
    return words;
}

char *parse_str(char *str)
{
    int first_quote = 0;
    int end_quote = 0;

    for(int i = 0; i < 256; i++){
	if(str[i] == '\"'){
	    first_quote = i;
	    break;
	}
    }
    for(int n = first_quote+1; n < 256; n++){
	if(str[n] == '\"'){
	    end_quote = n;
	    break;
	}
    }
    
    char *buf = (char*)malloc(sizeof(char)*((end_quote-first_quote)+1));
    int bufheap = 0;

    for(int j = first_quote+1; j < end_quote; j++){
	bool escape_found = false;
        if(str[j] == '\\'){
	    if(str[j+1] == 'n'){
		buf[bufheap] = '\n';
		bufheap++;
		j += 1;
	    }
	    else if(str[j+1] == 'b'){
		buf[bufheap] = '\b';
		bufheap++;
		j += 1;
	    }
	    else if(str[j+1] == '0'){
		buf[bufheap] = '\0';
		bufheap++;
		j += 1;
	    }
	    else if(str[j+1] == 't'){
		buf[bufheap] = '\t';
		bufheap++;
		j += 1;
	    }
	     else if(str[j+1] == '\\'){
		buf[bufheap] = '\\';
		bufheap++;
		j += 1;
	    }
	    escape_found = true;
	}
	if(escape_found == false){
	    buf[bufheap] = str[j];
	    bufheap++;
	}
    }
    
    buf[bufheap] = '\0';

    return buf;
}

bool check_gaps(char *str)
{
    bool gapped = false;
    for(int i = 0; i < 256; i++){
	if(str[i] == ' '){
	    gapped = true;
	}
    }
    return gapped;
}


Token *parse_token_from_file(const char* program)
{
    int p_word = file_word_count(program);
    Token *list = (Token*)malloc(sizeof(Token)*p_word);
    int heap = -1;

    FILE *pipe = fopen(program,"r");

    char *buffer = (char*)malloc(sizeof(char)*256);
    char chop[] = {' ','\n','\0'};
    int line = 0;
    
    while(fgets(buffer,256,pipe) != NULL){
	
	// Preserving the buffer for later usage
	char *prsv_buffer = (char*)malloc(sizeof(char)*256);
	for(int i = 0; i < 256; i++){
	    prsv_buffer[i] = buffer[i];
	}

       
	// String mechanism #TODO
	bool has_str = false;
	int col = 0;
	int str_at = 0;
	int ignore_till = 0;
	
	for(int i = 0; i < 256; i++){
	    if(prsv_buffer[i] == '\"'){
		str_at = i;

		for(int j = str_at+1; j < 256; j++){
		    if(prsv_buffer[j] == '\"'){
			ignore_till = j;
			has_str = true;
			break;
		    }
		}
		break;
	    }
	}

       
	// Checking the word gap of the String
	int gap_begin = 0;
	int gap_end = 0;
	int gaps_traverssed = 0;
	bool is_spaced = true;


	// ## Space-Gapped string error is because of the logic mistake here
	if(has_str == true){
	    for(int n = 0; n < ignore_till+1; n++){
		
		if(prsv_buffer[n] == ' '  && is_spaced == false){
		    gaps_traverssed += 1;
		    is_spaced = true;
		}
		
		else if(prsv_buffer[n] != ' '){
		    is_spaced = false;
		}
		if(n == str_at){
		    gap_begin = gaps_traverssed;
		}
		else if(n == ignore_till){
		    gap_end = gaps_traverssed;
		}
	    }
	    // printf("Gaps Traversed: BEGIN: %d END: %d -- %d\n",gap_begin,gap_end,line);
	}
	
	
	
	line++;
	char *token = strtok(buffer,chop);
	bool limit_reached = false;
	
	while(token != NULL){
	    if(starts_with(token,';') == true){
		break;
	    }
	    bool pass = false;
	    
	    char *mini_buffer = (char*)malloc(sizeof(char)*(strlen(token)+1));
	    int i = 0;

	    // Moving Memory fix
	    for(;token[i] != '\0';i++){
		mini_buffer[i] = token[i];
	    }
	    mini_buffer[i++] = '\0';


	    // #TODOOOOOOOOOOOOOOOOOOOOOOOOO: LEFT WORK WITH PARSING
	    // String mechanism
	    //col++;
	    if(has_str == true && (col < gap_begin || col > gap_end)){
		pass = true;
	    }
	    else if(has_str == false){
		pass = true;
	    }
	    if(pass == false && limit_reached == false){
		heap++;
		list[heap].raw = parse_str(prsv_buffer);
		list[heap].tt_type = STR;
		list[heap].line = line;
		limit_reached = true;
	    }


	    // Normally
	    if(pass == true){
		heap++;
		list[heap].raw = mini_buffer;
		list[heap].tt_type = check_token_type(list[heap].raw);
		list[heap].line = line;
	    }

	    token = strtok(NULL,chop);
	    col++;
	}
    }
    fclose(pipe);
    return list;
}

int cross_reference_else(Token *stack,int current)
{
    int loc = -1;
    int jmp_more = 0;

    int heap = 0;
    
    for(int i = current+1; stack[i].tt_type <= UNEXPECTED; i++){
	if(stack[i].tt_type == COMMENT){
	    break;
	}
	else if(stack[i].tt_type == IF){
	    heap++;
	}
	else if(stack[i].tt_type == ELSE){
	    if(heap != 0){
		heap--;
	    }
	    else{
		loc = i;
		break;
	    }
	}
    }
    return loc;
}
 
int cross_reference_end(Token *stack,int current)
{
    int loc = -1;
    int jmp_more = 0;
    
    for(int i = current+1; stack[i].tt_type <= UNEXPECTED; i++){
	if(stack[i].tt_type == COMMENT){
	    break;
	}
	else if(stack[i].tt_type == IF){
	    jmp_more += 1;
	}
	else if(stack[i].tt_type == WHILE){
	    jmp_more += 1;
	}
	else if(stack[i].tt_type == END){
	    if(jmp_more == 0){
	        loc = i;
		break;
	    }
	    else{
		jmp_more -= 1;
	    }
	}
    }
    return loc;
}




void simulate(const char* program,Token *program_ip,int argc,char **argv)
{
    printf("[INFO] Simulating File: %s\n\n",program);
    int pipe_word = file_word_count(program);
    int stack[pipe_word];
    int stack_heap = -1;

    int tc_stack[pipe_word];
    int tc_heap = -1;
    int end_stack[pipe_word];
    int end_heap = -1;
    int else_stack[pipe_word];
    int else_heap = -1;

    int while_bool_heap = -1;
    bool while_bool_stack[pipe_word];

    int while_loc = -1;
    int while_loc_stack[pipe_word];

    // Memory
    char memory[MEMORY_SIZE]; // Random Memory Read and Write Access (RMRWA)
    int memory_heap = 0;

    // end safety
    int expected_end = 0;
    int found_end = 0;


    char *strings[STRING_CAPACITY];
    int str_heap = -1;

    // end_type_Check
    TokenType endcheck[pipe_word/2];
    int endcheckheap = -1;

    for(int i = 0; program_ip[i].tt_type <= UNEXPECTED; i++){
	 if(program_ip[i].tt_type == INT){
	     stack_heap++;
	     stack[stack_heap] = atoi(program_ip[i].raw);
	 }
	 else if(program_ip[i].tt_type == STR){
	     str_heap++;
	     strings[str_heap] = program_ip[i].raw;
	 }
	 else if(program_ip[i].tt_type == WHILE){
	    int end = cross_reference_end(program_ip,i);
	    
	    if(end == -1){
		printf("%s:%d: Error: expected an end of the 'while' instruction\n",program,program_ip[i].line);
		printf("Error: simulation terminated\n");
		exit(1);
	    }
	    else{
		expected_end++;
		tc_heap++;
		tc_stack[tc_heap] = WHILE;
		end_heap++;
		end_stack[end_heap] = end;

	        while_bool_heap++;
		while_bool_stack[while_bool_heap] = true;

		while_loc++;
		while_loc_stack[while_loc] = i;

		endcheckheap++;
		endcheck[endcheckheap] = WHILE;
	    }
	}
	else if(program_ip[i].tt_type == ADD){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b + a;
	}
	else if(program_ip[i].tt_type == MIN){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b - a;
	} 
	else if(program_ip[i].tt_type == MUL){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b * a;
	}
	else if(program_ip[i].tt_type == DIV){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b / a;
	}
	else if(program_ip[i].tt_type == PRINT){
	    printf("%d\n",stack[stack_heap]);
	    stack_heap--; 
	}
	else if(program_ip[i].tt_type == DUP){
	    int a = stack[stack_heap];
	    stack_heap++;
	    stack[stack_heap] = a;
	}
	else if(program_ip[i].tt_type == MOD){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b % a;
	}
	else if(program_ip[i].tt_type == SWAP){
	    int temp = stack[stack_heap];
	    stack[stack_heap] = stack[stack_heap-1];
	    stack[stack_heap-1] = temp;
	}
	else if(program_ip[i].tt_type == DROP){
	    stack_heap--;
	}
	else if(program_ip[i].tt_type == OVER){
	    int element = stack[stack_heap-1];
	    stack_heap++;
	    stack[stack_heap] = element;
	}
	else if(program_ip[i].tt_type == ROT){
	    // x y z -- y z x
	    int z = stack[stack_heap];
	    stack_heap--;
	    int y = stack[stack_heap];
	    stack_heap--;
	    int x = stack[stack_heap];

	    stack[stack_heap] = y;
	    stack_heap++;
	    stack[stack_heap] = z;
	    stack_heap++;
	    stack[stack_heap] = x;
	}
	else if(program_ip[i].tt_type == NIP){
	    int a = stack[stack_heap];
	    stack_heap--;
	    stack[stack_heap] = a;
	}
	else if(program_ip[i].tt_type == DUPD){
	    int element = stack[stack_heap];
	    stack[stack_heap] = stack[stack_heap-1];
	    stack_heap++;
	    stack[stack_heap] = element;
	}
	else if(program_ip[i].tt_type == SWAPD){
	    int before1 = stack[stack_heap-1];
	    int before2 = stack[stack_heap-2];
	    stack[stack_heap-1] = before2;
	    stack[stack_heap-2] = before1;
	}
	else if(program_ip[i].tt_type == EQ){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b == a;
	}
	else if(program_ip[i].tt_type == NEQ){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b != a;
	}
	else if(program_ip[i].tt_type == GT){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b > a;
	}
	else if(program_ip[i].tt_type == LT){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b < a;
	}
	else if(program_ip[i].tt_type == GE){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b >= a;
	}
	else if(program_ip[i].tt_type == LE){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b <= a;
	}
	else if(program_ip[i].tt_type == BAND){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b & a;
	}
	else if(program_ip[i].tt_type == BOR){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b | a;
	}
	else if(program_ip[i].tt_type == BEOR){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b ^ a;
	}
	else if(program_ip[i].tt_type == SHL){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b << a;
	}
	else if(program_ip[i].tt_type == SHR){
	    int a = stack[stack_heap];
	    stack_heap--;
	    int b = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = b >> a;
	}
	else if(program_ip[i].tt_type == NOT){
	    int a = stack[stack_heap];
	    stack_heap--;

	    stack_heap++;
	    stack[stack_heap] = !a;
	}
	else if(program_ip[i].tt_type == IF){
	    int end = cross_reference_end(program_ip,i);
	    int elz = cross_reference_else(program_ip,i);

	    if(end == -1){
		printf("%s:%d: Error: expected an end of the 'if' instruction\n",program,program_ip[i].line);
		printf("Error: simulation terminated\n");
		exit(1);
	    }
	    else{
		// if-end safety
		expected_end++;
		
		tc_heap++;
		tc_stack[tc_heap] = IF;
		end_heap++;
		end_stack[end_heap] = end;
	        else_heap++;
	        else_stack[else_heap] = elz;

		endcheckheap++;
		endcheck[endcheckheap] = IF;
		continue;
	    }
	}
	 
	else if(program_ip[i].tt_type == THEN){
	    int last_block = tc_stack[tc_heap];
	    tc_heap--;
	    int jmp_end = end_stack[end_heap];
	    end_heap--;

	   	    
	    if(last_block == WHILE){
		int condition = stack[stack_heap];
		stack_heap--;

		if(condition == true){
		}
		else if(condition == false){
		    while_bool_stack[while_bool_heap] = false;
		    i = jmp_end-1;
		}
	    }
	    else if(last_block == IF){
	    	int condition = stack[stack_heap];
		stack_heap--;

		if(condition == true){
		}
		else if(condition == false){
		    int elz = else_stack[else_heap];
		    else_heap--;

		    if(elz != -1){
			i = elz;
		    }
		    else{
			i = jmp_end-1;
		    }
		}
	    }
	}

      
	else if(program_ip[i].tt_type == END){
	    //printf("Attended End! Loc: %d\n",i);
	    found_end++;

	    if(found_end > expected_end){
		printf("%s:%d: Error: there is no reference to the beginning of this instruction ending with 'end'\n",program,program_ip[i].line);
		printf("Error: simulation terminated\n");
		exit(1);
	    }

	    TokenType last = endcheck[endcheckheap];
	    endcheckheap--;

	    if(last == WHILE){
		bool last_while_bool = while_bool_stack[while_bool_heap];
		while_bool_heap--;

		if(last_while_bool == true){
		    int jmp_back = while_loc_stack[while_loc];
		    while_loc--;

		    i = jmp_back - 1;
		}
	    }
	    else if(last == IF){
		continue;
	    }
	}
       else if(program_ip[i].tt_type == ELSE){
	    int end = cross_reference_end(program_ip,i);
	    i = end;
       }
       else if(program_ip[i].tt_type == MEMORY){
	   char value = (char)stack[stack_heap];
	   stack_heap--;
	   int ptr = stack[stack_heap];
	   stack_heap--;

	   memory_heap += 1;

	   memory[ptr] = value;
       }
       else if(program_ip[i].tt_type == MEMPTR){
	   stack_heap++;
	   stack[stack_heap] = memory_heap;
       }
       else if(program_ip[i].tt_type == MEMREAD){
	   int ptr = stack[stack_heap];
	   stack_heap--;

	   stack_heap++;
	   stack[stack_heap] = memory[ptr];
       }
       else if(program_ip[i].tt_type == WRITE){
	   int len = stack[stack_heap];
	   stack_heap--;
	   int buf = stack[stack_heap];
	   stack_heap--;
	   int fd = stack[stack_heap];
	   stack_heap--;

	   if(fd == 1){
	       for(int j = buf;j < len; j++){
		   printf("%c",memory[j]);
	       }
	   }
	   else if(fd == 2){
	       for(int j = buf;j < len; j++){
		   fprintf(stderr,"%c",memory[j]);
	       }
	   }
	   else{
	       printf("%s:%d: Error: Unknown file descriptor in 'write' function call %d\n",program,program_ip[i].line,fd);
	       printf("Error: simulation terminated\n");
	       exit(1);
	   }
       }
       else if(program_ip[i].tt_type == PUTS){
	   printf("%s",strings[str_heap]);
	   str_heap--;
       }
       else if(program_ip[i].tt_type == EXIT){
	   int estat = stack[stack_heap];
	   stack_heap--;

	   exit(estat);
       }
       else if(program_ip[i].tt_type == STRCMP){
	   char *a = strings[str_heap];
	   str_heap--;
	   char *b = strings[str_heap];
	   str_heap--;

	   stack_heap++;
	   stack[stack_heap] = (strcmp(b,a) == 0) ? true : false;
       }
       else if(program_ip[i].tt_type == STRLEN){
	   char *a = strings[str_heap];
	   str_heap--;

	   stack_heap++;
	   stack[stack_heap] = strlen(a);
       }
	 // Parser doesn't even parses this
       else if(program_ip[i].tt_type == COMMENT){
	    int force_push = 0;
	    int current = program_ip[i].line;
	    bool lines_left_to_lex = false;
	    int j;
	    
	    for(j = i; program_ip[j].tt_type <= UNEXPECTED; j++){
		if(program_ip[j].line > current){
		    lines_left_to_lex = true;
		    force_push = j;
		    break;
		}
	    }
	    if(lines_left_to_lex == true){
		i = force_push-1; // Fix for skipping the element at : force_push
	    }
	    else{
		break;
	    }
	}
	else if(program_ip[i].tt_type == UNEXPECTED){
	    printf("%s:%d: Error: unexpected word found '%s'\n",program,program_ip[i].line,program_ip[i].raw);
	    printf("Error: simulation terminated\n");
	    exit(1);
	}
    }
}


void analyse(const char* program,Token *program_ip)
{
    printf("[INFO] LEXICAL ANALYSIS: \n");
    printf("[INFO] FILE: %s\n",program);

    for(int i = 0; program_ip[i].tt_type <= UNEXPECTED; i++){
	printf("Token: %s\n",program_ip[i].raw);
	printf("TokenType: %s\n",tt_sign[program_ip[i].tt_type]);
    }
}


int main(int argc,char **argv)
{
    if(argc < 3){
        usage();
	exit(1);
    }
    else{
	// Options
	bool visible_time = false;
	
        char *mode = argv[1];
	char *base_file = argv[2];

	if(extension_check(base_file) == false){
	    printf("ERROR: unknown file format provided: '%s'\n",base_file);
	    exit(1);
	}

	// travelling the argv to check if there are options
	for(int i = 0; i < argc; i++){
	    if(strcmp(argv[i],"-t") == 0){
		visible_time = true;
	    }
	}

	FILE *test_pipe = fopen(base_file,"r");
	if(!test_pipe){
	    printf("ERROR: provided an invalid input file '%s'\n",base_file);
	    usage();
	    exit(1);
	}
	fclose(test_pipe);
	
        if(strcmp(mode,"-s") == 0){
	    clock_t t;
	    Token *list = parse_token_from_file(base_file);

	    t = clock();
	    simulate(base_file,list,argc,argv);
	    double tt = ((double)t)/CLOCKS_PER_SEC;

	    if(visible_time == true){
		printf("\nSimulation finished in %lf seconds\n",tt);
	    }
	}
	else if(strcmp(mode,"-l") == 0){
	    clock_t t;
	    Token *list = parse_token_from_file(base_file);

	    t = clock();
	    analyse(base_file,list);
	    double tt = ((double)t)/CLOCKS_PER_SEC;

	    if(visible_time == true){
		printf("\nLexical report produced in %lf seconds\n",tt);
	    }
	}
	else{
	    printf("ERROR: provided an unexpected mode '%s'\n",mode);
	    usage();
	    exit(1);
	}
    }
    
    return 0;
}
