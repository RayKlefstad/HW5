
#define pushSymbolListList(sy, l) (l) = new SymbolListPair(sy, l)
#define popSymbolListList(l) (l) = (l)->next
#define topSymbolListList(l) (l)->info

class SymTab
{
    SymbolListList head;
    stringList names;
protected:
    void enterSymbol(Symbol sy); // puts symbol in top scope
    Symbol findSymbolInTopScope(string name); // looks only in top scope
public:
    SymTab()
    {
        head = 0;
        names = 0; // for debugging, save name of each scope
        enterScope("TOP LEVEL");
        enterSymbol(TypeSymbol::make("void", VoidType :: make()));
        enterSymbol(TypeSymbol::make("str", StrType :: make()));
        enterSymbol(TypeSymbol::make("int", IntType :: make()));
        enterSymbol(TypeSymbol::make("bool", BoolType :: make()));
        enterSymbol(TypeSymbol::make("any", AnyType :: make()));
    }
    ~SymTab()
    {
        if (head) exitScope();
    }
    void enterScope(string name, SymbolList syli = 0); // enters syli into new top scope
    SymbolList exitScope(); // returns symbols removed from top scope
    SymbolListList topScope() { return head; }
    Symbol findSymbol(string name); // returns visible declaration for name
    void declare(Symbol sy); // handles object declarations with checking
    static Symbol findSymbolInList(string name, SymbolList sl);

    static void putSymbolList(ostream &out, SymbolList L);
    void put(ostream & out); // print out a symbol table for debugging
};

inline ostream & operator << (ostream & out, SymTab &st)
{
    st.put(out); return out;
}

extern SymTab ST;
