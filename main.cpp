void check(StmtList L)
{
    for (StmtList p = L; p; p=p->next)
        p->info->check();
}

void do_homework(StmtList L)
{
    switch (HW)
    {
        case 3: cout << L << endl; break;
        case 4:
        case 5: check(L); break;
        default:
            compiler_error("Unknown homework option");
    }
}

int scan1_main()
{
    while (int token = yylex())
    {
        if (token != Newline && token != Indent && token != Dedent && token != EndMarker)
        {
            cout << "Token = " << token_image(token) << "\t\t";
            cout << "Lexeme = \"" << yytext << "\"";
            cout << endl;
        }
    }
    return 0;
}

int scan2_main()
{
    while (int token = yylex())
    {
        cout << "Token = " << token_image(token) << "\t\t";
        if (token != Newline && token != Indent && token != Dedent && token != EndMarker)
            cout << "Lexeme = \"" << yytext << "\"";
        cout << endl;
    }
    return 0;
}

void parse_main()
{
    yyparse();
}

int main(int argc, char *argv[])
{
    int opt;
    while (true)
        switch ( opt = getopt(argc, argv, "0123456789") )
        {
            case '0':
                scan1_main();
                break;
            case '1':
                scan2_main();
                break;
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                HW = opt - '0';
                parse_main();
                break;
            case -1:
                exit(0);
            default:
                cerr << "Unknown program option: " << static_cast<char>(opt) << endl;
        }
    return 0;
}
