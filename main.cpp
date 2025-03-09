#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include <regex>
#include <fstream>
using namespace std;

enum TokenType {
    IDENTIFIER, KEYWORD, OPERATOR, SEPARATOR, NUMBER, COMMENT, FORMAT_SPECIFIER, FUNCTION_CALL, UNKNOWN, LITERAL
};

bool isKeyword(const string& str) {

    static unordered_set<string> keywords = { "if", "else", "int", "float", "return", "while", "for", "char", "void", "do", "break", "continue", "switch", "case", "default", "goto", "auto", "const", "double", "enum", "extern", "long", "signed", "sizeof", "static", "struct", "typedef", "unsigned" };
    return keywords.find(str) != keywords.end();
}

bool isFunction(const string& str) {
    return str == "scanf" || str == "printf";
}

bool isOperator(const string& str) {
    static unordered_set<string> operators = { "+", "-", "*", "/", "=", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "+=", "-=", "*=", "/=", "++", "--", "&", "|", "^", "~", "<<", ">>"};
    return operators.find(str) != operators.end();
}

bool isSeparator(char ch) {
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';' || ch == ',' || ch == '[' || ch == ']';
}

bool isFormatSpecifier(const string& str) {
    regex formatRegex("%[difs]");
    return regex_match(str, formatRegex);
}

void outputToken(const string& value, TokenType type, int line, int length, int totalLines=0) {
    string typeStr;
    switch (type) {
        case IDENTIFIER: typeStr = "IDENTIFIER"; break;
        case KEYWORD:    typeStr = "KEYWORD"; break;
        case OPERATOR:   typeStr = "OPERATOR"; break;
        case SEPARATOR:  typeStr = "SEPARATOR"; break;
        case NUMBER:    typeStr = "NUMBER"; break;
        case COMMENT:    typeStr = "COMMENT"; break;
        case LITERAL:    typeStr = "LITERAL"; break;
        case FORMAT_SPECIFIER: typeStr = "FORMAT_SPECIFIER"; break;
        case FUNCTION_CALL: typeStr = "FUNCTION_CALL"; break;
        case UNKNOWN:    typeStr = "UNKNOWN"; break;
    }
    if (totalLines == 0)
    cout << R"(Token: ")" << value << R"(" , Type: )" << typeStr 
              << ", Line: " << line << ", Length: " << length << endl;
    else
    cout << R"(Token: ")" << value << R"(" , Type: )" << typeStr 
              << ", Lines: " << line << "-" << line+totalLines << ", Length: " << length << endl;
}

void handleFormatSpecifiers(const string& formatString, int line) {
    string currentToken;
    
    for (size_t i = 0; i < formatString.size(); i++) {
        if (formatString[i] == '%') {
            currentToken += formatString[i];
            currentToken += formatString[i + 1];
            if (isFormatSpecifier(currentToken)) {
                outputToken(currentToken, FORMAT_SPECIFIER, line, 2);
            }
            currentToken.clear();
            i++;
        }
    }
}

void lex(const string& code) {
    int lineNumber = 1;
    string currentToken;

    for (size_t i = 0; i < code.size(); i++) {
        char ch = code[i];
        
        if (ch == '\n') {
            lineNumber++;
            continue;
        }

        if (ch == '"') {
            currentToken += ch;
            i++;
            while (i < code.size()) {
                ch = code[i];
                currentToken += ch;
                
                if (ch == '\\' && i + 1 < code.size()) {
                    currentToken += code[++i];
                } else if (ch == '"') { 
                    break;
                }
                
                i++;
            }

            
            outputToken(currentToken, LITERAL, lineNumber, currentToken.size());
            currentToken.clear();
            continue; 
        }

        if (isalpha(ch)) {
            currentToken += ch;
            while (i + 1 < code.size() && isalnum(code[i + 1])) {
                currentToken += code[++i];
            }
            if (isKeyword(currentToken)) {
                outputToken(currentToken, KEYWORD, lineNumber, currentToken.size());
            } 
            else if (isFunction(currentToken)) {
                outputToken(currentToken, FUNCTION_CALL, lineNumber, currentToken.size());
                

                if (code[i + 1] == '(') {
                    i++;
                    while (code[++i] != '"' && i < code.size());
                    string formatString;
                    while (code[++i] != '"' && i < code.size()) {
                        formatString += code[i];
                    }
                    handleFormatSpecifiers(formatString, lineNumber);
                }
            } 
            else {
                outputToken(currentToken, IDENTIFIER, lineNumber, currentToken.size());
            }
            currentToken.clear();
        } 
            else if (isdigit(ch)) {
            currentToken += ch;
            bool isUnknown = false;
            while (i + 1 < code.size() && (isdigit(code[i + 1]) || isalpha(code[i + 1]))) {
                currentToken += code[++i];
                if (isalpha(code[i])) {
                    isUnknown = true;
                }
            }
            if (isUnknown) {
                outputToken(currentToken, UNKNOWN, lineNumber, currentToken.size());
            } else {
                outputToken(currentToken, NUMBER, lineNumber, currentToken.size());
            }
            currentToken.clear();
        }
        else if (isSeparator(ch)) {
            currentToken += ch;
            outputToken(currentToken, SEPARATOR, lineNumber, 1);
            currentToken.clear();
        } 
        
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '=') {
            currentToken += ch;
            if (i + 1 < code.size() && (code[i + 1] != ' ' && code [i+1] != ';') ) 
                currentToken += code[++i];
            
            if (currentToken == "/*")
            {
                currentToken.clear();
                i++;
                int sLine = lineNumber;
                int commentLines = 0;
                while (commentLines == 0) {
                    if (code[i] != '\n' && code[i] != '\t') 
                        currentToken += code[i];
                    if (code[i] == '\n') 
                        lineNumber++;
                    if (code[i] == '*' && code[i+1] == '/') 
                    commentLines = 1;
                    i++;
                }
                currentToken[currentToken.size()-1] = '\0';
                int eLine = lineNumber;
                outputToken(currentToken, COMMENT, lineNumber, currentToken.size(), eLine-sLine);
                currentToken.clear();
            }
            if (ch == '"' )
            {
                currentToken.clear();
                i++;
                int sLine = lineNumber;
                int commentLines = 0;
                while (commentLines == 0) {
                    if (code[i] != '\n' && code[i] != '\t') 
                        currentToken += code[i];
                    if (code[i] == '\n') 
                        lineNumber++;
                    if (code[i] == '"') 
                    commentLines = 1;
                    i++;
                }
                currentToken[currentToken.size()-1] = '\0';
                int eLine = lineNumber;
                if( lineNumber - sLine > 1)
                    outputToken(currentToken, LITERAL, lineNumber, currentToken.size(), eLine-sLine);
                else
                    outputToken(currentToken, LITERAL, lineNumber, currentToken.size());
                currentToken.clear();
            }
            if (currentToken == "//") {
                currentToken.clear();
                i++;
                while (i < code.size() && code[i] != '\n') {
                    currentToken += code[i];
                    i++;
                }
                outputToken(currentToken, COMMENT, lineNumber, currentToken.size());
                currentToken.clear();
            }
            else if(isOperator(currentToken))
                outputToken(currentToken, OPERATOR, lineNumber, currentToken.size());
            else
                outputToken(currentToken, UNKNOWN, lineNumber, currentToken.size());
            currentToken.clear();
        }
    }
}

int main() {
    ifstream file("input.in");
    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    cout << "Code:\n" << code << endl;
    file.close();
    lex(code);
    return 0;
}

// input.in

/*

int main() 
{ 
    int num, a12b34, 13aaa; 
    string x= "un string" 
    printf("Enter:");     
    scanf("%d", &num); 

    // true if num is perfectly divisible by 2 
    if(num % 2 == 0) 
        printf("%d is even.", num); 
    else 
        printf("%d is odd.", num);

    return 0;  
} 

*/