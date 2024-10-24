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
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, lineNumber, columnNumber});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, lineNumber, columnNumber});
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
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement(); // Add while parsing
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement(); // Add for parsing
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

    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }

    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        expect(T_ID); // Initialize the loop variable
        expect(T_ASSIGN);
        parseExpression(); // Initialize value
        expect(T_SEMICOLON);
        parseExpression(); // Loop condition
        expect(T_SEMICOLON);
        expect(T_ID); // Update variable
        expect(T_ASSIGN);
        parseExpression(); // Update value
        expect(T_RPAREN);
        parseStatement();
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT)
        {
            pos++;
            parseExpression();
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

int main()
{
    string input = R"(
    int a;
    a = 0;
    while (a < 5) {
        a = a + 1;
    }

    for (int i = 0; i < 10; i = i + 1) {
        return i;
    }

    if (a > 5) {
        return a;
    } else {
        return 0;
    }
)";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
