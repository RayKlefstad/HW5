inline void requireFuncType(Type t)
{
    require(t->behavior(isFunc), "function type");
}

inline void requireClassType(Type t)
{
    require(t->behavior(isClass), "class type");
}

inline void requireListType(Type t)
{
    require(t->behavior(isList), "list type");
}

inline void requireIntType(Type t)
{
    require(t->behavior(isInt), "int type");
}

inline void requireSameType(Type t1, Type t2)
{
    if (DEBUG) cout << "requireSameType: T1 = " << t1 << " T2 = " << t2 << endl;
    require(t1->isSameType(t2), "same type");
}

inline void requireSameIntType(Type t1, Type t2)
{
    require(t1->behavior(isInt) && t2->behavior(isInt), "same int type");
}

inline void requireIntOrStrType(Type ty)
{
    require(ty->behavior(isInt) || ty->behavior(isStr), "int or str type");
}

inline void requireBoolType(Type t)
{
    require(t->behavior(isBool), "bool type");
}

inline void requireBothBoolType(Type t1, Type t2)
{
    require(t1->behavior(isBool) && t2->behavior(isBool), "bool type");
}

inline void requireArgMatch(SymbolList f, ExprList a)
{
    for (; f && a; f = f->next, a = a->next)
        require(f->info->type->isSameType(a->info->type), "argument type match");
    require(!a, "less arguments");
    require(!f, "more arguments");
}

inline void requireAllSameType(ExprList a)
{
    if (!a) return;
    Type ty = a->info->type;
    for (ExprList p = a->next; p; p = p->next)
        requireSameType(ty, p->info->type);
}

inline void requireLocation(Expr e)
{
    require(e->isLocation(), "location");
}
