#include <string>
#include <numeric>
#include "llvm/Support/Casting.h"

#include "generator.h"


// void VarGen::GenValue(std::string &str) {
//     str = name;
//     str << ".write ";
//     str << value;
// }


SpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::staticSpaceGenPtr = nullptr;
std::string AbstractGen::filename;
std::map <uint64_t, std::string> AbstractGen::identifiers = std::map<uint64_t, std::string>();
int AbstractGen::shift = 0;

SourceGen::~SourceGen() {
    delete glob;
    delete stat;
}


//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::ostream &out) {
    out << type << " " << value << " > " << name;
}

//--------------------------------------------------------------------------------------------------
/// shift is indentation level. shift 2 is equal to 4 spaces
std::string StmtGen::getIndentSpaces() {

    return std::string(shift * 2, ' ');
}

//--------------------------------------------------------------------------------------------------
void MultiLineStmtGen::Add(StmtGen* stmt) {
    UnaryStmtGen* st = llvm::dyn_cast<UnaryStmtGen>(stmt);
    //TODO Вынести Empty как отдельный тип или метод
    if (st && st->op == "" && st->value == "" && st->nestedStmt == nullptr)
        return;
    statements.push_back(stmt);
}

void MultiLineStmtGen::Generate(std::ostream &out) {
    int lines = 0;

    for (int i = 0; i < statements.size(); ++i) {
        if (!llvm::isa<MultiLineStmtGen>(statements[i]))
            out << getIndentSpaces();
        out << statements[i];
        if (i + 1 != statements.size() || llvm::isa<EmptyStmtGen>(statements[i])) {
            out << "\n";
            lines++;
        }
    }


}

MultiLineStmtGen::~MultiLineStmtGen() {
    statements.clear();
}


void CompoundStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    out << value;
    out << "\n";
    AbstractGen::shift++;
    if (!statements.empty()) {
        MultiLineStmtGen::Generate(out);
        out << "\n";
    }
    out << getIndentSpaces();
    out << "TRUE";
    AbstractGen::shift--;
}

//--------------------------------------------------------------------------------------------------
void FuncGen::Generate(std::ostream &out) {
    // Первоначально осуществляется генерация списка атрибутов
    out << "[";
    if (name == "main") {
        out << "arg] > main\n";
    } else {
        int paramCounter = 0;
        for (const auto &paramName: paramNames) {
            ++paramCounter; // с лишним пробелом в параметрах не компилируется
            if(paramCounter != paramName.size()) {
                out << " ";
            }

            out << paramName;
        }
        out << "] > ";
//        out << "ret_param_xxxx] > ";
        out << name;
        out << "\n";
    }
    // Далее идет формирование тела функции

    AbstractGen::shift++;
    if(!returnType.empty()) { // проверка на void
        out << getIndentSpaces() << returnType << " > ret_param_xxxx";
        out << "\n";
    }
    out << body;
    AbstractGen::shift--;
    out << "\n";
    if (name == "main") {
        out << "main arg > @\n";
    }
}

void BinaryStmtGen::Generate(std::ostream &out) {
    //str << value +"(";
    bool leftLinear = isLeftLinear(left);
    if (!leftLinear)
        out << "(";
    out << left;
    if (!leftLinear)
        out << ")";
    out << value + "(";
    out << right;
    out << ")";

}

BinaryStmtGen::~BinaryStmtGen() {
    delete BinaryStmtGen::left;
    delete BinaryStmtGen::right;
}

bool BinaryStmtGen::isLeftLinear(StmtGen* pGen) {
    if (pGen == nullptr || !llvm::isa<UnaryStmtGen>(pGen))
        return false;
    auto* unaryStmtGen = static_cast<UnaryStmtGen*> (pGen);
    if (unaryStmtGen->nestedStmt != nullptr)
        return isLeftLinear(unaryStmtGen->nestedStmt);
    return true;
}


UnaryStmtGen::~UnaryStmtGen() {
    delete UnaryStmtGen::nestedStmt;
}

void UnaryStmtGen::Generate(std::ostream &out) {
    bool empty = value.empty() || nestedStmt == nullptr;
    out << value;
    if (!empty)
        out << "(";
    if (nestedStmt != nullptr)
        nestedStmt->Generate(out);
    out << postfix;
    if (!empty)
        out << ")";
}

//--------------------------------------------------------------------------------------------------
void SpaceGen::Generate(std::ostream &out) {
    // Формирование списка глобальных объектов
    //
    for (auto globalObject: objects) {
        out << globalObject;
        out << "\n";
    }

}

//........................................................................
// Добавление очередного объекта к глобальному пространству
void SpaceGen::Add(AbstractGen* obj) {
    objects.push_back(obj);
}

SpaceGen::~SpaceGen() {
    objects.clear();
}


void IfStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    out << "if.\n";
    AbstractGen::shift++;
    MultiLineStmtGen::Generate(out);
    AbstractGen::shift--;
}

IfStmtGen::~IfStmtGen() {
}

//--------------------------------------------------------------------------------------------------
void RecordGen::Generate(std::ostream &out) {
    out << "[";
    if (!fields.empty())
        out << "field_init_0";
    for (size_t i = 1; i < count; i++)
        out << " field_init_" << i;
    out << "] > " << name;
    out << "\n  ";
    out << "\"" << type << "\" > type\n";
    size_t j = 0;
    for (RecordGen* rg: fields) {
        out << "\n  " << rg->type << " ";
        for (size_t i = 0; i < rg->count; i++, j++)
            out << "field_init_" << j << " ";
        out << "> " << rg->name;
    }
    if (!fields.empty())
        out << "\n";
    out << "\n  " << "[value] > write\n" << "    seq > @";
    for (RecordGen* vg: fields)
        out << "\n      ^." << vg->name << ".write (value." << vg->name << ")";
    out << "\n      TRUE\n";
}