// *** SYMBOL ***

typedef struct SymbolBlock * Symbol;

typedef ListPair<Symbol> SymbolPair;
typedef SymbolPair * SymbolList;

typedef ListPair<SymbolList> SymbolListPair;
typedef SymbolListPair * SymbolListList;

Symbol findSymbolInList(string name, SymbolList sl);

typedef struct TypeBlock * Type;

typedef ListPair<Type> TypePair;
typedef TypePair * TypeList;

enum TypeBehavior {isBool, isInt, isStr, isList, isFunc, isClass, isAny};

inline int length(SymbolList L)
{
    int len = 0;
    for (; L; L=L->next)
        ++len;
    return len;
}

struct TypeBlock
{
    Type type;
    string name;

    TypeBlock(string nm)
        : type(0), name(nm)
    {
    }

    virtual void put(ostream & out)
    {
        out << name;
    }


    static Type make()
    {
        compiler_error("Attempt to create instance of abstract base class TypeBlock");
        return 0;
    }

    virtual void check()
    {
        compiler_error("Undefined member function: TypeBlock :: check");
    }

    virtual bool behavior(TypeBehavior b)
    {
        return false;
    }

    Type rootType(Type ty)
    {
        while (ty->type)
            ty = ty->type;
        return ty;
    }

    virtual bool isSameType(Type ty)
    {
        Type t1 = rootType(this);
        Type t2 = rootType(ty);
        if (DEBUG) cout << "2) isSameType: T1 = " << t1->name << " T2 = " << t2->name << endl;
        if ( t1 == t2 )
            return true;
        if ( t1->behavior(isAny) || t2->behavior(isAny) )
            return true;
        return false;
    }

};

inline void checkType(Type t)
{
    if (t) t->check();
}


inline ostream & operator << (ostream & out, Type t)
{
    if (t)
        t->put(out);
    else
        out << "NULL";
    return out;
}


inline void put(ostream &out, TypeList types)
{
    for (TypeList p = types; p; p=p->next)
    {
        out << p->info;
        if (p->next) out << ", ";
    }
}


inline ostream & operator << (ostream & out, TypeList l)
{
    if (l)
        put(out, l);
    else
        out << "NULL";
    return out;
}



struct IdentType
    : TypeBlock
{
    IdentType(string nm)
        : TypeBlock(nm)
    {
    }

    static Type make(string nm)
    {
        
        return new IdentType(nm);;
    }

    virtual void put(ostream & out)
    {
        if (type != this)
            out << type;
        else
            out << name;
    }

    virtual void check();

    virtual bool behavior(TypeBehavior b)
    {
        if (type)
            return type->behavior(b);
        else
            return false;
    }
};


struct BoolType
    : TypeBlock
{

    BoolType()
        : TypeBlock("Bool")
    {
    }

    static Type make()
    {
        static Type t = 0;
        if (!t) t = new BoolType();
        return t;
    }

    virtual void check()
    {
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isBool;
    }
};

struct IntType
    : TypeBlock
{

    IntType()
        : TypeBlock("Int")
    {
    }

    static Type make()
    {
        static Type t = 0;
        if (!t) t = new IntType();
        return t;
    }

    virtual void check()
    {
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isInt;
    }

};

struct StrType
    : TypeBlock
{

    StrType()
        : TypeBlock("Str")
    {
    }

    static Type make()
    {
        static Type t = 0;
        if (!t) t = new StrType();
        return t;
    }

    virtual void check()
    {
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isStr;
    }

};

struct VoidType
    : TypeBlock
{

    VoidType()
        : TypeBlock("Void")
    {
    }

    static Type make()
    {
        static Type t = 0;
        if (!t) t = new VoidType();
        return t;
    }

    virtual void check()
    {
    }

    virtual bool behavior(TypeBehavior b)
    {
        return false;
    }
};

struct AnyType
    : TypeBlock
{

    AnyType()
        : TypeBlock("Any")
    {
    }

    static Type make()
    {
        static Type t = 0;
        if (!t) t = new AnyType();
        return t;
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isAny;
    }

    virtual void check()
    {
    }

    virtual bool isSameType(Type ty)
    {
        return true;
    }
};

struct ListType
    : TypeBlock
{
    Type elementType;

    ListType(Type et)
        : TypeBlock("List"), elementType(et)
    {
    }

    static Type make(Type et)
    {
        
        return new ListType(et);
    }

    virtual void put(ostream & out)
    {
        out << '[' << elementType << ']';
    }

    virtual void check()
    {
        checkType(elementType);
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isList;
    }

    virtual bool isSameType(Type ty)
    {
        ListType * t = dynamic_cast<ListType*>(ty);
        if (!t) return false;
        return elementType->isSameType(t->elementType);
    }
};

struct FuncType
    : TypeBlock
{
    string name;
    SymbolList params;
    Type ret_type;

    FuncType(string nm, SymbolList pms, Type rt)
        : TypeBlock("Func"), name(nm), params(pms), ret_type(rt)
    {
    }

    static Type make(string name, SymbolList params, Type ret_type)
    {
        return new FuncType(name, params, ret_type);
    }

    virtual void put(ostream & out)
    {
        out << name << '(' << params << ')';
        if (ret_type)
            out << "->" << ret_type;
    }

    virtual void check()
    {
        // params and return type are checked() in Stmt
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isFunc;
    }


    static bool paramsTypesMatch(SymbolList L1, SymbolList L2);

    virtual bool isSameType(Type ty)
    {
        FuncType * t = dynamic_cast<FuncType*>(ty);
        if (!t)
            return false;
        if (!ret_type->isSameType(t->ret_type))
            return false;
        if (!paramsTypesMatch(params, t->params))
            return false;
        return true;
    }
};

struct ClassType
    : TypeBlock
{
    SymbolList members;
    SymbolListList scopeHolder;

    ClassType(SymbolList m)
        : TypeBlock("Class"), members(m)
    {
    }

    static Type make(SymbolList s)
    {
        return new ClassType(s);
    }

    virtual void put(ostream & out)
    {
        out << "Class " << name << " [" << length(scopeHolder->info) << "]";
    }

    virtual void check()
    {
        // TBD?
    }

    virtual bool behavior(TypeBehavior b)
    {
        return b == isClass;
    }

    Symbol findMember(string name)
    {
        return findSymbolInList(name, scopeHolder->info);
    }
};

struct UndefinedType
    : TypeBlock
{
    UndefinedType()
        : TypeBlock("Undefined")
    {
    }

    static Type make()
    {
        return new UndefinedType();
    }

    virtual void put(ostream & out)
    {
        out << "UndefinedType";
    }

    virtual void check()
    {
    }

    virtual bool behavior(TypeBehavior b)
    {
        return true;
    }
};


// SYMBOLBLOCK

struct SymbolBlock
{
    string name;
    Type type;

    SymbolBlock(string nm, Type ty)
        : name(nm), type(ty)
    {
    }

    virtual void put(ostream & out)
    {
        out << name << ":" << type;
    }

    static Symbol make(SymbolList s)
    {
        compiler_error("undefined method put for SymbolBlock :: make()");
        return 0;
    }


};

inline void putSymbolList(ostream & out, SymbolList l)
{
    for (SymbolList p = l; p; p=p->next)
    {
        p->info->put(out);
        if (p->next)
            out << ", ";
    }
}

inline ostream & operator << (ostream & out, Symbol s)
{
    if (s)
        s->put(out);
    else
        out << "NULL";
    return out;
}

inline ostream & operator << (ostream & out, SymbolList L)
{
    if (L)
        putSymbolList(out, L);
    else
        out << "NULL";
    return out;
}


struct VarSymbol
    : SymbolBlock
{

    VarSymbol(string n, Type rt)
        : SymbolBlock(n, rt)
    {
    }

    static Symbol make(string n, Type rt)
    {
        return new VarSymbol(n, rt);
    }


};

struct ParamSymbol
    : SymbolBlock
{

    ParamSymbol(string n, Type rt)
        : SymbolBlock(n, rt)
    {
    }

    static Symbol make(string n, Type rt)
    {
        return new ParamSymbol(n, rt);
    }


};

struct TypeSymbol
    : SymbolBlock
{
    TypeSymbol(string n, Type rt)
        : SymbolBlock(n, rt)
    {
        // rt->name = n;
    }

    static Symbol make(string n, Type rt)
    {
        return new TypeSymbol(n, rt);
    }


};

struct ClassSymbol
    : SymbolBlock
{
    SymbolListList scopeHolder;

    ClassSymbol(string n, Type rt)
        : SymbolBlock(n, rt), scopeHolder(0)
    {
        rt->name = n;
    }

    static Symbol make(string n, Type rt)
    {
        return new ClassSymbol(n, rt);
    }


};

struct FuncSymbol
    : SymbolBlock
{
    SymbolList params;

    FuncSymbol(string n, SymbolList prms, Type rt)
        : SymbolBlock(n, rt), params(prms)
    {
        rt->name = n;
    }

    static Symbol make(string n, SymbolList prms, Type rt)
    {
        return new FuncSymbol(n, prms, rt);
    }


};

struct UndefinedSymbol
    : SymbolBlock
{
    UndefinedSymbol()
        : SymbolBlock("Undefined", UndefinedType::make())
    {
    }

    static Symbol make()
    {
        return new UndefinedSymbol();
    }


};
