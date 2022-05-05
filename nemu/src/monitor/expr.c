#include "nemu.h"
#include "cpu/reg.h"
extern CPU_STATE cpu;
#include "memory/memory.h"
#include "monitor/breakpoint.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
	NOTYPE = 256,
	EQ,   // 257
	UEQ,  // 258
	LEQ,  // 259
	GEQ,  // 260
	LSH,  // 261
	RSH,  // 262
	AND,  // 263
	OR,   // 264
	REV,  // 265
	NUM,  // 266
	NUM2, // 267
	NUM8, // 268
	NUM16,// 269
	REG,  // 270
	REG16,// 271
	REG8, // 272
	SYMB  // 273

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE},
	{"%eax|%ecx|%edx|%ebx|%esp|%ebp|%esi|%edi|%eip", REG},
	{"%ax|%cx|%dx|%bx|%sp|%bp|%si|%di", REG16},
	{"%ah|%al|%ch|%cl|%dh|%dl|%bh|%bl", REG8},
	{"\\$eax|\\$ecx|\\$edx|\\$ebx|\\$esp|\\$ebp|\\$esi|\\$edi|\\$eip", REG},
	{"\\$ax|\\$cx|\\$dx|\\$bx|\\$sp|\\$bp|\\$si|\\$di", REG16},
	{"\\$ah|\\$al|\\$ch|\\$cl|\\$dh|\\$dl|\\$bh|\\$bl", REG8},
	{"==", EQ},
	{"!=", UEQ},
	{"<=", LEQ},
	{">=", GEQ},
	{"<<", LSH},
	{">>", RSH},
	{"&&", AND},
	{"\\|\\|", OR},
	{"\\+", '+'},
	{"-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"&", '&'},
	{"\\|", '|'},
	{"\\^", '^'},
	{"!", '!'},
	{"~", '~'},
	{"<", '<'},
	{">", '>'},
	{"%", '%'},
	{"0[Xx][0-9a-fA-F]+", NUM16},
	{"[0-9a-fA-F]+[Hh]", NUM16},
	{"0[Bb][0-1]+", NUM2},
	{"[0-1]+[Bb]", NUM2},
	{"0[0-7]+", NUM8},
	{"[0-7]+[Oo]", NUM8},
	{"[0-9]+", NUM},
	{"[a-zA-Z_][a-zA-Z0-9_]+", SYMB},
	{"[a-zA-Z_]", SYMB}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0 && pmatch.rm_eo > 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				// printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				case NOTYPE:
					break;
				default:
					tokens[nr_token].type = rules[i].token_type;
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

static uint32_t check_brackets(int s, int e){
	int count = 0;
	for(int i = s; i <= e; ++i){
		if(tokens[i].type == '('){
			count++;
		}
		else if(tokens[i].type == ')'){
			count--;
		}
		if(count < 0){
			return 0;
		}
	}
	return (count == 0);
}

uint32_t eval(int s, int e, bool *success){
	if(s > e || *success == false){
		*success = false;
		return -1;
	}
	if(s == e){
		uint32_t tempnum = 0;
		uint32_t i = 0, ei = strlen(tokens[s].str);
		switch(tokens[s].type){
			case NUM:{
				tempnum = atoi(tokens[s].str);
				break;
			}
			case NUM2:{
				if(tokens[s].str[1] == 'b' || tokens[s].str[1] == 'B')
					i = 2;
				else
					--ei;
				for(; i < ei; ++i){
					tempnum <<= 1;
					tempnum += (tokens[s].str[i] - '0');
				}
				break;
			}
			case NUM8:{
				if(tokens[s].str[ei - 1] == 'o' || tokens[s].str[ei - 1] == 'O')
					--ei;
				for(; i < ei; ++i){
					tempnum <<= 3;
					tempnum += (tokens[s].str[i] - '0');
				}
				break;
			}
			case NUM16:{
				if(tokens[s].str[ei - 1] == 'h' || tokens[s].str[ei - 1] == 'H')
					--ei;
				else
					i = 2;
				for(; i < ei; ++i){
					tempnum <<= 4;
					if(tokens[s].str[i] >= '0' && tokens[s].str[i] <= '9')
						tempnum += (tokens[s].str[i] - '0');
					else if(tokens[s].str[i] >= 'a' && tokens[s].str[i] <= 'f')
						tempnum += (tokens[s].str[i] - 'a' + 10);
					else if(tokens[s].str[i] >= 'A' && tokens[s].str[i] <= 'F')
						tempnum += (tokens[s].str[i] - 'A' + 10);
				}
				break;
			}
			case REG:{
				if(tokens[s].str[3] == 'x'){
					if(tokens[s].str[2] == 'a')
						tempnum = cpu.eax;
					else if(tokens[s].str[2] == 'c')
						tempnum = cpu.ecx;
					else if(tokens[s].str[2] == 'd')
						tempnum = cpu.edx;
					else tempnum = cpu.ebx;
				}else if(tokens[s].str[3] == 'p'){
					if(tokens[s].str[2] == 's')
						tempnum = cpu.esp;
					else if(tokens[s].str[2] == 'b')
						tempnum = cpu.ebp;
					else tempnum = cpu.eip;
				}else{
					if(tokens[s].str[2] == 's')
						tempnum = cpu.esi;
					else tempnum = cpu.edi;
				}
				break;
			}
			case REG16:{
				if(tokens[s].str[2] == 'x'){
					if(tokens[s].str[1] == 'a')
						tempnum = cpu.gpr[0]._16;
					else if(tokens[s].str[1] == 'c')
						tempnum = cpu.gpr[1]._16;
					else if(tokens[s].str[1] == 'd')
						tempnum = cpu.gpr[2]._16;
					else tempnum = cpu.gpr[3]._16;
				}else if(tokens[s].str[2] == 'p'){
					if(tokens[s].str[1] == 's')
						tempnum = cpu.gpr[4]._16;
					else tempnum = cpu.gpr[5]._16;
				}else{
					if(tokens[s].str[1] == 's')
						tempnum = cpu.gpr[6]._16;
					else tempnum = cpu.gpr[7]._16;
				}
				break;
			}
			case REG8:{
				int p1, p2 = (tokens[s].str[2] == 'h') ? 1 : 0;
				if(tokens[s].str[1] == 'a')
					p1 = 0;
				else if(tokens[s].str[1] == 'c')
					p1 = 1;
				else if(tokens[s].str[1] == 'd')
					p1 = 2;
				else p1 = 3;
				tempnum = cpu.gpr[p1]._8[p2];
				break;
			}
			case SYMB:{
				tempnum = look_up_symtab(tokens[s].str, success);
				if(!(*success))
					return -1;
				break;
			}
			default:{
				*success = false;
				return -1;
			}
		}
		return tempnum;
	}
	if(check_brackets(s, e) == 0){
		*success = false;
		return -1;
	}
	if(tokens[s].type == '(' && tokens[e].type == ')' && check_brackets(s + 1, e - 1) != 0){
		return eval(s + 1, e - 1, success);
	}
	int pos_op = -1, nowLevel = -1;
	for(int i = s; i <= e; ++i){
		// 跳过括号
		if(tokens[i].type == '('){
			int count = 1;
			do{
				++i;
				if(tokens[i].type == '(')
					++count;
				else if(tokens[i].type == ')')
					--count;
			}while(count != 0);
		}
		if(nowLevel <= 2 && (tokens[i].type == '!' || tokens[i].type == '~')){
			pos_op = i;
			nowLevel = 2;
			continue;
		}
		if(nowLevel <= 2 && tokens[i].type == '-' && i == s){
			pos_op = i;
			nowLevel = 2;
			continue;
		}
		if(nowLevel <= 3 && (tokens[i].type == '*' || tokens[i].type == '/' || tokens[i].type == '%')){
			pos_op = i;
			nowLevel = 3;
			continue;
		}
		if(nowLevel <= 4 && (tokens[i].type == '+' || tokens[i].type == '-')){
			pos_op = i;
			nowLevel = 4;
			continue;
		}
		if(nowLevel <= 5 && (tokens[i].type == LSH || tokens[i].type == RSH)){
			pos_op = i;
			nowLevel = 5;
			continue;
		}
		if(nowLevel <= 6 && (tokens[i].type == '>' || tokens[i].type == '<' || tokens[i].type == GEQ || tokens[i].type == LEQ)){
			pos_op = i;
			nowLevel = 6;
			continue;
		}
		if(nowLevel <= 7 && (tokens[i].type == EQ || tokens[i].type == UEQ)){
			pos_op = i;
			nowLevel = 7;
			continue;
		}
		if(nowLevel <= 8 && (tokens[i].type == '&')){
			pos_op = i;
			nowLevel = 8;
			continue;
		}
		if(nowLevel <= 9 && (tokens[i].type == '^')){
			pos_op = i;
			nowLevel = 9;
			continue;
		}
		if(nowLevel <= 10 && (tokens[i].type == '|')){
			pos_op = i;
			nowLevel = 10;
			continue;
		}
		if(nowLevel <= 11 && (tokens[i].type == AND)){
			pos_op = i;
			nowLevel = 11;
			continue;
		}
		if(nowLevel <= 12 && (tokens[i].type == OR)){
			pos_op = i;
			nowLevel = 12;
			continue;
		}
	}
	switch (tokens[pos_op].type){
		case '!':{
			return !eval(pos_op + 1, e, success);
			break;
		}
		case '~':{
			return ~eval(pos_op + 1, e, success);
			break;
		}
		case '-':{
			if(nowLevel == 2){
				return -eval(pos_op + 1, e, success);
			}else return eval(s, pos_op - 1, success) - eval(pos_op + 1, e, success);
			break;
		}
		case '*':{
			return eval(s, pos_op - 1, success) * eval(pos_op + 1, e, success);
			break;
		}
		case '/':{
			return eval(s, pos_op - 1, success) / eval(pos_op + 1, e, success);
			break;
		}
		case '%':{
			return eval(s, pos_op - 1, success) % eval(pos_op + 1, e, success);
			break;
		}
		case '+':{
			return eval(s, pos_op - 1, success) + eval(pos_op + 1, e, success);
			break;
		}
		case LSH:{
			return eval(s, pos_op - 1, success) << eval(pos_op + 1, e, success);
			break;
		}
		case RSH:{
			return eval(s, pos_op - 1, success) >> eval(pos_op + 1, e, success);
			break;
		}
		case '>':{
			return eval(s, pos_op - 1, success) > eval(pos_op + 1, e, success);
			break;
		}
		case '<':{
			return eval(s, pos_op - 1, success) < eval(pos_op + 1, e, success);
			break;
		}
		case GEQ:{
			return eval(s, pos_op - 1, success) >= eval(pos_op + 1, e, success);
			break;
		}
		case LEQ:{
			return eval(s, pos_op - 1, success) <= eval(pos_op + 1, e, success);
			break;
		}
		case EQ:{
			return eval(s, pos_op - 1, success) == eval(pos_op + 1, e, success);
			break;
		}
		case UEQ:{
			return eval(s, pos_op - 1, success) != eval(pos_op + 1, e, success);
			break;
		}
		case '&':{
			return eval(s, pos_op - 1, success) & eval(pos_op + 1, e, success);
			break;
		}
		case '^':{
			return eval(s, pos_op - 1, success) ^ eval(pos_op + 1, e, success);
			break;
		}
		case '|':{
			return eval(s, pos_op - 1, success) | eval(pos_op + 1, e, success);
			break;
		}
		case AND:{
			return eval(s, pos_op - 1, success) && eval(pos_op + 1, e, success);
			break;
		}
		case OR:{
			return eval(s, pos_op - 1, success) || eval(pos_op + 1, e, success);
			break;
		}
		default:{
			*success = false;
			return -1;
		}
	}
	return 0;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}

	// printf("\nPlease implement expr at expr.c\n");
	// fflush(stdout);
	// assert(0);

	return eval(0, nr_token - 1, success);
}
