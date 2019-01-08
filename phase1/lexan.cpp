#include <cctype>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum {
    KEYWORD, ID, INTEGER, REAL, STRING, OPERATOR, DONE,
};

static std::vector<std::string> keywords = {
	"auto", "break", "case", "char", "const", 
	"continue", "default", "do", "double", "else", "enum", 
	"extern", "float", "for", "goto", "if", "int", "long",
	"register", "return", "short", "signed", "sizeof", 
	"static", "struct", "switch", "typedef", "union", 
	"unsigned", "void", "volatile", "while"
};

int isKeyword(string s){
	for(unsigned int i = 0; i < keywords.size(); i++){
		if(s == keywords[i]){
			return 1;
		}
	}
	return 0;
}

static vector<string> operators2 = {
	"||",
	"&&",
	"==",
	"!=",
	"<=",
	">=",
	"++",
	"--",
	"->"
};

int isOperator2(string s){
	for(unsigned int i = 0; i < operators2.size(); i++){
		if(s == operators2[i]){
			return 1;
		}
	}
	return 0;
}

//set of characters that could be a operator1 or operator2
static vector<char> operators00 = {
	'<',
	'>',
	'!',
	'&',
	'=',
	'+',
	'-'
};

int isOperator00(char c){
	for(unsigned int i = 0; i < operators00.size(); i++){
		if(c == operators00[i]){
			return 1;
		}
	}
	return 0;
}

static vector<char> operators1 = {
	'*',
	'/',
	'%',
	'.',
	'(',
	')',
	'[',
	']',
	'{',
	'}',
	';',
	':',
	','
};

int isOperator1(char c){
	for(unsigned int i = 0; i < operators1.size(); i++){
		if(c == operators1[i]){
			return 1;
		}
	}
	return 0;
}

int isignored(char character){
	switch(character){
		case ' ':
		case '\t':
		case '\n':
		case '\f':
		case '\v':
		case '\r':
			return 1;
		default:
			return 0;
	}
}

int lexan(string& lexbuf){

	char input = cin.peek();
	char cbuff;
	string opbuff;

	while(!cin.eof()){

		lexbuf.clear();

		//handle whitespace or otherwise ignored characters
		if(isignored(input)){
			do {
				cin.get();
				input = cin.peek();
			} while(isignored(input));
		}

		//handle numbers
		if(isdigit(input)){
			do {
				lexbuf += input;
				cin.get();
				input = cin.peek();
			} while(isdigit(input));

			if(input != '.'){
				return INTEGER;
			}
			else{
				//I am guaranteed a digit here in a well-formed program.
				do {
					lexbuf += input;
					cin.get();
					input = cin.peek();
				} while(isdigit(input));

				//if = real
				if(input != 'e' && input != 'E'){
					return REAL;
				}
				//else = real with exponent:
				else{
					
					lexbuf += input;
					cin.get();
					input = cin.peek();

					if(input == '+' || input == '-'){
						lexbuf += input;
						cin.get();
						input = cin.peek();
					}

					//I can assume (given well-formed program) that the next input will be a digit. Get the rest of those and then return real.
					do {
						lexbuf += input;
						cin.get();
						input = cin.peek();
					} while(isdigit(input));

					return REAL;

				}

			}
			//Check for an [eE] immediately afterwards
		}

		//handle identifiers
		if(input == '_'){
			do {
				lexbuf += input;
				cin.get();
				input = cin.peek();
			} while( input == ' ' || isalpha(input) || isdigit(input));
			return ID;
		}

		//handle identifiers OR keywords
		if(isalpha(input)){
			do {
				lexbuf += input;
				cin.get();
				input = cin.peek();
			} while(input == '_' || isalpha(input) || isdigit(input));

			if(isKeyword(lexbuf)){
				return KEYWORD;
			}
			else{
				return ID;
			}
		}

		//handle strings
		if(input == '"'){
			do {
				lexbuf += input;
				cin.get();
				input = cin.peek();
			} while(input != '"');
			
			//add the second "
			lexbuf += input;
			cin.get();
			input = cin.peek();

			return STRING;
		}

		//handle comments
		if(input == '/'){
			cin.get();
			input = cin.peek();

			if(input == '*'){
				do {
commentLoop:
					cin.get();
					input = cin.peek();
				} while(input != '*');

				//add the start to the input, look at the next char
				cin.get();
				input = cin.peek();
				//if its anything but the / char, send it back into the do-while loop (logic preserved)
				if(input != '/'){
					goto commentLoop;
				}

				//removes / from cin
				cin.get();
				input = cin.peek();

			}
			else{
				cin.putback('/');
				input = '/';
			}
		}

		//handle operators
		if(isOperator1(input)){
			lexbuf += input;
			cin.get();
			input = cin.peek();

			return OPERATOR;
		}

		//Ambiguous operator; could be 1 or two chars long
		if(isOperator00(input)){
			cbuff = input;
			cin.get();
			input = cin.peek();

			opbuff += cbuff;
			opbuff += input;

			if(isOperator2(opbuff)){
				lexbuf += opbuff;
				cin.get();
				input = cin.peek();
				return OPERATOR;
			}
			else{
				lexbuf += cbuff;
				return OPERATOR;
			}

		}
		
		//What should I do with illegal characters?
		cin.get();
		input = cin.peek();

	}

	return DONE;

}


int main()
{
    int token;
    string lexbuf, type;

    while ((token = lexan(lexbuf)) != DONE) {
	switch(token) {
	case ID:
	    type = "identifier";
	    break;

	case KEYWORD:
	    type = "keyword";
	    break;

	case INTEGER:
	    type = "integer";
	    break;

	case REAL:
	    type = "real";
	    break;

	case STRING:
	    type = "string";
	    break;

	case OPERATOR:
	    type = "operator";
	    break;
	}

	cout << type << ":" << lexbuf << endl;
    }

    return 0;
}


