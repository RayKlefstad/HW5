extern int row;
#define DEBUG 0

inline void lexical_error(char c)
{
    cout << "*** Lexical Error " << row << ": " << c << endl;
}

inline void compiler_error(string s)
{
    cout << "*** Fatal Error:" << s << endl;
}

inline void syntax_error(string s)
{
    cout << "*** Syntax Error " << row << ":" << s << endl;
}

inline void semantic_error(string s)
{
    cout << "*** Semantic Error:" << s << endl;
}

#define yyerror(s) syntax_error(s)


inline void require(bool cond, string msg)
{
    if (!cond)
        cout << "*** Semantic Error:" << msg << " required" << endl;
}

inline void debug(string msg)
{
    if (DEBUG) cout << "--- DEBUG: " << msg << endl;
}
