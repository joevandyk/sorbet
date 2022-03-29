#include <fstream>

constexpr int MAX_PROC_ARITY = 10;

using namespace std;

void emitProc(ofstream &out, int arity) {
    out << "class Proc" << arity << " < Proc" << '\n';
    out << "  Return = type_member(:out)" << '\n';
    for (int i = 0; i < arity; ++i) {
        out << "  Arg" << i << " = type_member(:in)" << '\n';
    }

    out << '\n';

    if (arity > 0) {
        out << "  sig do" << '\n';
        out << "    params(" << '\n';
    } else {
        out << "  sig {";
    }
    for (int i = 0; i < arity; ++i) {
        out << "      arg" << i << ": "
            << "Arg" << i << "," << '\n';
    }
    if (arity > 0) {
        out << "    )" << '\n';
        out << "    .returns(Return)" << '\n';
        out << "  end" << '\n';
    } else {
        out << "returns(Return)}" << '\n';
    }
    out << "  def call(";
    for (int i = 0; i < arity; ++i) {
        if (i != 0) {
            out << ", ";
        }
        out << "arg" << i;
    }
    out << "); end" << '\n';
    out << '\n';
    out << "  alias_method :[], :call" << '\n';
    out << "end" << '\n' << '\n';
}

int main(int argc, char **argv) {
    ofstream rb(argv[1], ios::trunc);
    rb << "# typed: strict" << '\n';
    for (int arity = 0; arity <= MAX_PROC_ARITY; ++arity) {
        emitProc(rb, arity);
    }
}
