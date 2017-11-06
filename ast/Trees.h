#ifndef SRUBY_TREES_H
#define SRUBY_TREES_H

#include "../common/common.h"
#include "Context.h"
#include "Symbols.h"
#include <memory>
#include <vector>

namespace ruby_typer {
namespace ast {

class Statement {
public:
    Statement() = default;
    virtual ~Statement() = default;
    virtual std::string toString(GlobalState &gs, int tabs = 0) = 0;
    virtual std::string nodeName() = 0;
    virtual std::string showRaw(GlobalState &gs, int tabs = 0) = 0;
    Loc loc;
};

class Expression : public Statement {
public:
    static std::unique_ptr<Expression> fromStatement(std::unique_ptr<Statement> &expr);
    static std::unique_ptr<Expression> fromStatement(std::unique_ptr<Statement> &&expr);
};

class Reference : public Expression {};

class ControlFlow : public Expression {};

class Declaration : public Expression {
public:
    SymbolRef symbol;

    Declaration(SymbolRef symbol);
};

enum ClassDefKind : u1 { Module, Class };

class ClassDef : public Declaration {
public:
    inline SymbolRef parent(Context ctx) {
        return symbol.info(ctx).parent(ctx);
    }

    inline std::vector<SymbolRef> &mixins(Context ctx) {
        return symbol.info(ctx).mixins(ctx);
    }

    std::vector<std::unique_ptr<Statement>> rhs;
    std::unique_ptr<Expression> name;
    // For unresolved names. Once they are resolved to Symbols they go into the
    // Symbol
    std::vector<std::unique_ptr<Expression>> ancestors;
    ClassDefKind kind;

    ClassDef(SymbolRef symbol, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> &ancestors,
             std::vector<std::unique_ptr<Statement>> &rhs, ClassDefKind kind);

    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);

    virtual std::string nodeName();
};

class MethodDef : public Declaration {
public:
    std::unique_ptr<Expression> rhs;
    std::vector<std::unique_ptr<Expression>> args;
    NameRef name;
    bool isSelf;

    MethodDef(SymbolRef symbol, NameRef name, std::vector<std::unique_ptr<Expression>> &args,
              std::unique_ptr<Expression> rhs, bool isSelf);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class ConstDef : public Declaration {
public:
    std::unique_ptr<Expression> rhs;

    ConstDef(SymbolRef symbol, std::unique_ptr<Expression> rhs);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class If : public ControlFlow {
public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Expression> thenp;
    std::unique_ptr<Expression> elsep;
    If(std::unique_ptr<Expression> cond, std::unique_ptr<Expression> thenp, std::unique_ptr<Expression> elsep);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class While : public ControlFlow {
public:
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Statement> body;

    While(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> body);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class For : public ControlFlow {
    // TODO
};

class Break : public ControlFlow {
public:
    std::unique_ptr<Expression> expr;

    Break(std::unique_ptr<Expression> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Next : public ControlFlow {
public:
    std::unique_ptr<Expression> expr;

    Next(std::unique_ptr<Expression> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Return : public ControlFlow {
public:
    std::unique_ptr<Expression> expr;

    Return(std::unique_ptr<Expression> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Yield : public ControlFlow {
public:
    std::unique_ptr<Expression> expr;

    Yield(std::unique_ptr<Expression> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Rescue : public ControlFlow {
public:
    std::unique_ptr<Expression> body;
    SymbolRef binder;
    SymbolRef binder_type;
    std::unique_ptr<Expression> handler;

    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Ident : public Reference {
public:
    SymbolRef symbol;
    NameRef name;

    Ident(SymbolRef symbol);
    Ident(NameRef name, SymbolRef symbol);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class RestArg : public Reference {
public:
    std::unique_ptr<Reference> expr;

    RestArg(std::unique_ptr<Reference> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class KeywordArg : public Reference {
public:
    std::unique_ptr<Reference> expr;

    KeywordArg(std::unique_ptr<Reference> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class OptionalArg : public Reference {
public:
    std::unique_ptr<Reference> expr;
    std::unique_ptr<Expression> default_;

    OptionalArg(std::unique_ptr<Reference> expr, std::unique_ptr<Expression> default_);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Nil : public Expression {
public:
    Nil(){};
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class SymbolLit : public Expression {
public:
    NameRef name;

    SymbolLit(NameRef name);

    virtual std::string toString(GlobalState &gs, int tabs);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Assign : public Expression {
public:
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;

    Assign(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Block;

class Send : public Expression {
public:
    std::unique_ptr<Expression> recv;
    NameRef fun;
    std::vector<std::unique_ptr<Expression>> args;
    u4 flags = 0;

    static const int PRIVATE_OK = 1 << 0;

    // null if no block passed
    std::unique_ptr<Block> block;

    Send(std::unique_ptr<Expression> recv, NameRef fun, std::vector<std::unique_ptr<Expression>> &&args);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class New : public Expression {
public:
    SymbolRef claz;
    std::vector<std::unique_ptr<Expression>> args;

    New(SymbolRef claz, std::vector<std::unique_ptr<Expression>> &&args);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Super : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> args;

    Super(std::vector<std::unique_ptr<Expression>> &&args);

    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class NamedArg : public Expression {
public:
    NameRef name;
    std::unique_ptr<Expression> arg;

    NamedArg(NameRef name, std::unique_ptr<Expression> arg);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Hash : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> keys;
    std::vector<std::unique_ptr<Expression>> values;

    Hash(std::vector<std::unique_ptr<Expression>> &keys, std::vector<std::unique_ptr<Expression>> &values);

    virtual std::string toString(GlobalState &gs, int tabs);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Array : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> elems;

    Array(std::vector<std::unique_ptr<Expression>> &elems);

    virtual std::string toString(GlobalState &gs, int tabs);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class FloatLit : public Expression {
public:
    float value;

    FloatLit(float value);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class IntLit : public Expression {
public:
    int value;

    IntLit(int value);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class StringLit : public Expression {
public:
    NameRef value;

    StringLit(NameRef value);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class BoolLit : public Expression {
public:
    bool value;

    BoolLit(bool value);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class ConstantLit : public Expression {
public:
    NameRef cnst;
    std::unique_ptr<Expression> scope;

    ConstantLit(std::unique_ptr<Expression> scope, NameRef cnst);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class ArraySplat : public Expression {
public:
    std::unique_ptr<Expression> arg;

    ArraySplat(std::unique_ptr<Expression> arg);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class HashSplat : public Expression {
public:
    std::unique_ptr<Expression> arg;

    HashSplat(std::unique_ptr<Expression> arg);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Self : public Expression {
public:
    SymbolRef claz;

    Self(SymbolRef claz);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class Block : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> args;
    std::unique_ptr<Expression> body;

    Block(std::vector<std::unique_ptr<Expression>> &args, std::unique_ptr<Expression> body);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class InsSeq : public Expression {
public:
    std::vector<std::unique_ptr<Statement>> stats;
    std::unique_ptr<Expression> expr;

    InsSeq(std::vector<std::unique_ptr<Statement>> &&stats, std::unique_ptr<Expression> expr);
    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class EmptyTree : public Expression {

    virtual std::string toString(GlobalState &gs, int tabs = 0);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

class NotSupported : public Expression {
    std::string why;

public:
    NotSupported(const std::string &why);

    virtual std::string toString(GlobalState &gs, int tabs);
    virtual std::string showRaw(GlobalState &gs, int tabs = 0);
    virtual std::string nodeName();
};

/** https://git.corp.stripe.com/gist/nelhage/51564501674174da24822e60ad770f64
 *
 *  [] - prototype only
 *
 *                 / Control Flow <- while, if, for, break, next, return, rescue, case
 * Pre-CFG-Node <-
 *                 \ Instruction <- assign, send, [new], ident, named_arg, hash, array, literals(symbols, ints, floats,
 * strings, constants, nil), constants(resolver will desugar it into literals), array_splat(*), hash_splat(**), self,
 * insseq, Block)
 *
 *                  \ Definition  <-  class(name, parent, mixins, body)
 *                                    module
 *                                    def
 *                                    defself
 *                                    const_assign
 *
 *
 *
 * know id for: top, bottom, kernel?, basicobject, class, module [postponed], unit, Hash, Array, String, Symbol, float,
 * int, numeric, double, unknown
 *
 *
 *
 * Desugar string concatenation into series of .to_s calls and string concatenations
 */

} // namespace ast
} // namespace ruby_typer

#endif // SRUBY_TREES_H
