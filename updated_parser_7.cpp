#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>

using namespace std;

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_GT,
    T_LT,
    T_EQ,
    T_LE,
    T_GE,
    T_NEQ,
    T_AND,
    T_OR,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_COMMA,
    T_FOR,
    T_WHILE,
    T_DO,
    T_BREAK,
    T_CONTINUE,
    T_SEMICOLON,
    T_EOF,
    T_FLOAT,
    T_STRING,
    T_LOGICAL_AND, // Add logical AND token
    T_LOGICAL_OR,  // Add logical OR token
    T_EQUAL,       // Add equality token
    T_NOT_EQUAL,   // Add not equal token
};

struct Token
{
    TokenType type;
    string value;
    int lineNumber;
    int columnNumber;

    Token(TokenType type, const string &value, int lineNumber, int columnNumber)
        : type(type), value(value), lineNumber(lineNumber), columnNumber(columnNumber) {}
};

class Lexer
{
private:
    string src;
    size_t pos;
    int lineNumber;
    int columnNumber;

public:
    Lexer(const string &src) : src(src), pos(0), lineNumber(1), columnNumber(1) {}

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (isspace(current))
            {
                handleWhitespace(current);
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), lineNumber, columnNumber});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, lineNumber, columnNumber});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, lineNumber, columnNumber});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, lineNumber, columnNumber});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, lineNumber, columnNumber});
                else
                    tokens.push_back(Token{T_ID, word, lineNumber, columnNumber});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber, columnNumber});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", lineNumber, columnNumber});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", lineNumber, columnNumber});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", lineNumber, columnNumber});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", lineNumber, columnNumber});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", lineNumber, columnNumber});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", lineNumber, columnNumber});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", lineNumber, columnNumber});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", lineNumber, columnNumber});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber, columnNumber});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", lineNumber, columnNumber});
                break;

            // Other cases...
            case '&':
                if (src[pos + 1] == '&')
                {
                    tokens.push_back(Token{T_LOGICAL_AND, "&&", lineNumber, columnNumber});
                    pos++; // Skip the next '&'
                    columnNumber++;
                }
                else
                {
                    // Handle unexpected character
                    cout << "Unexpected character: " << current << " at line " << lineNumber << ", column " << columnNumber << endl;
                    exit(1);
                }
                break;
            case '|':
                if (src[pos + 1] == '|')
                {
                    tokens.push_back(Token{T_LOGICAL_OR, "||", lineNumber, columnNumber});
                    pos++; // Skip the next '|'
                    columnNumber++;
                }
                else
                {
                    // Handle unexpected character
                    cout << "Unexpected character: " << current << " at line " << lineNumber << ", column " << columnNumber << endl;
                    exit(1);
                }
                break;
            case '=':
                if (src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_EQUAL, "==", lineNumber, columnNumber});
                    pos++; // Skip the next '='
                    columnNumber++;
                }
                else
                {
                    tokens.push_back(Token{T_ASSIGN, "=", lineNumber, columnNumber});
                }
                break;
            case '!':
                if (src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_NOT_EQUAL, "!=", lineNumber, columnNumber});
                    pos++; // Skip the next '='
                    columnNumber++;
                }
                else
                {
                    // Handle unexpected character
                    cout << "Unexpected character: " << current << " at line " << lineNumber << ", column " << columnNumber << endl;
                    exit(1);
                }
                break;
                // Other cases...

            default:
                cout << "Unexpected character: " << current << " at line " << lineNumber << ", column " << columnNumber << endl;
                exit(1);
            }
            pos++;
            columnNumber++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber, columnNumber});
        return tokens;
    }

    void handleWhitespace(char current)
    {
        if (current == '\n')
        {
            lineNumber++;
            columnNumber = 1;
        }
        else
        {
            columnNumber++;
        }
        pos++;
    }

    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos]))
        {
            pos++;
            columnNumber++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
        {
            pos++;
            columnNumber++;
        }
        return src.substr(start, pos - start);
    }
};

class Parser
{
public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

    void parseStatement()
    {
        if (tokens[pos].type == T_INT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value << " at line " << tokens[pos].lineNumber << ", column " << tokens[pos].columnNumber << endl;
            exit(1);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration()
    {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        parseLogicalOr(); // Start with logical OR
    }

    void parseLogicalOr()
    {
        parseLogicalAnd(); // Process logical AND first
        while (tokens[pos].type == T_LOGICAL_OR)
        {
            pos++;             // Consume '||'
            parseLogicalAnd(); // Process the next logical AND
        }
    }

    void parseLogicalAnd()
    {
        parseComparison(); // Process comparisons first
        while (tokens[pos].type == T_LOGICAL_AND)
        {
            pos++;             // Consume '&&'
            parseComparison(); // Process the next comparison
        }
    }

    void parseComparison()
    {
        parseTerm();
        while (tokens[pos].type == T_EQ || tokens[pos].type == T_NEQ ||
               tokens[pos].type == T_GT || tokens[pos].type == T_LT ||
               tokens[pos].type == T_LE || tokens[pos].type == T_GE)
        {
            pos++;       // Consume the comparison operator
            parseTerm(); // Process the next term
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++;
            parseFactor();
        }
    }

    void parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value << " at line " << tokens[pos].lineNumber << ", column " << tokens[pos].columnNumber << endl;
            exit(1);
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected " << type << " but found " << tokens[pos].value << " at line " << tokens[pos].lineNumber << ", column " << tokens[pos].columnNumber << endl;
            exit(1);
        }
    }
};

int main() {
    string input = R"(
        int a;
        a = 5;
        int b;
        b = a + 10;
        if (b > 10 && a == 5) {
            return b;
        } else {
            return 0;
        }
        if (a != 5 || b <= 15) {
            return a;
        } else {
            return 1;
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
