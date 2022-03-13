#include "transpile_helper.h"
#include "unit_transpiler.h"
#include <queue>
using namespace clang;
using namespace llvm;
using namespace std;

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator);

EOObject GetIfStmtEOObject(const IfStmt *p_stmt);
EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  const CompoundStmt* funcBody = dyn_cast<CompoundStmt>(FD->getBody());
  if(!funcBody)
    return EOObject(EOObjectType::EO_EMPTY);
  return GetCompoundStmt(funcBody);
}
//Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS) {
  EOObject res {"seq","@"};
  for (auto stmt : CS->body() ) {
    Stmt::StmtClass stmtClass = stmt->getStmtClass();
    // Костыльное решение для тестового выводо
    if (stmtClass == Stmt::ImplicitCastExprClass) // Нужно разобраться с именами перчислимых типов
    {
      EOObject printer {"printf"};
      printer.nested.emplace_back(R"("%d"\n)",EOObjectType::EO_LITERAL);
      EOObject read_val {"read-as-int64"};
      extern UnitTranspiler transpiler;
      //TODO fix unsafety code
      read_val.nested.emplace_back(transpiler.glob.GetVarByID(
          reinterpret_cast<uint64_t>(dyn_cast<DeclRefExpr>(*stmt->child_begin())->getFoundDecl())).alias);
      printer.nested.push_back(read_val);
      res.nested.push_back(printer);
      continue;
    }
    EOObject stmt_obj = GetStmtEOObject(stmt);
    res.nested.push_back(stmt_obj);
  }
  res.nested.emplace_back("TRUE",EOObjectType::EO_LITERAL);
  return res;
}

EOObject GetStmtEOObject(const Stmt* stmt) {
  Stmt::StmtClass stmtClass = stmt->getStmtClass();
  if (stmtClass == Stmt::BinaryOperatorClass) {
    const auto* op = dyn_cast<BinaryOperator>(stmt);
    return GetBinaryStmtEOObject(op);
  } else if (stmtClass == Stmt::ParenExprClass) {
    const auto* op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::ImplicitCastExprClass) {
    const auto* op = dyn_cast<ImplicitCastExpr>(stmt);
    //TODO if cast kinds and also split it to another func
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::DeclRefExprClass) {
    const auto *op = dyn_cast<DeclRefExpr>(stmt);
    extern UnitTranspiler transpiler;
    //TODO fix unsafety code
    return EOObject{transpiler.glob.GetVarByID(reinterpret_cast<uint64_t>(op->getFoundDecl())).alias};
  } else if (stmtClass == Stmt::IfStmtClass) {
    const auto* op = dyn_cast<IfStmt>(stmt);
    return GetIfStmtEOObject(op);
  }
  return EOObject(EOObjectType::EO_EMPTY);
}
EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  string type_name = "-"+GetTypeName(p_operator->getType());
  auto opCode = p_operator->getOpcode();
  std::string operation;
  if (opCode == BinaryOperatorKind::BO_Assign) {
    operation = "write";
  } else if (opCode == BinaryOperatorKind::BO_Add) {
    operation = "add" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Sub) {
    operation = "sub" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Mul) {
    operation = "mul" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Div) {
    operation = "div" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Rem) {
    operation = "mod" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_And) {
    operation = "bit-and" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Or) {
    operation = "bit-or" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Xor) {
    operation = "bit-xor" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Shl) {
    operation = "shift-left" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_Shr) {
    operation = "shift-right" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_EQ) {
    operation = "eq" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_NE) {
    operation = "neq" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_LT) {
    operation = "less" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_LE) {
    operation = "leq" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_GT) {
    operation = "greater" + type_name;
  } else if (opCode == BinaryOperatorKind::BO_GE) {
    operation = "geq" + type_name;
  } else {
    operation = "undefined";
  }

  EOObject binop {operation};
  binop.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}

EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_stmt {"if"};
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getThen()));
  if (p_stmt->hasElseStorage()) {
    if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getElse()));
  } else {
    EOObject empty_seq {"seq"};
    empty_seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
    if_stmt.nested.push_back(empty_seq);
  }
  return if_stmt;
}

std::string GetTypeName(QualType qualType)
{
  extern ASTContext* context;
  const clang::Type* typePtr = qualType.getTypePtr();
  TypeInfo typeInfo = context->getTypeInfo(typePtr);
  uint64_t typeSize = typeInfo.Width;
  std::string str;

  if (typePtr->isBooleanType()) {
    str += "bool";
    return str;
  }
  if (typePtr->isFloatingType()) {
    str += "float" + std::to_string(typeSize);
    return str;
  }

  if (!typePtr->isSignedIntegerType())
    str += "u";
  if (typePtr->isCharType()) {
    str += "char";
    return str;
  }
  if (typePtr->isIntegerType()) {
    str += "int" + std::to_string(typeSize);
    return str;
  }


  if (typePtr->isUnionType())
    str = "un_";
  if (typePtr->isStructureType())
    str = "st_";
  if (typePtr->isUnionType() || typePtr->isStructureType()) {
    RecordDecl* RD = typePtr->getAsRecordDecl();
    if (RD->hasNameForLinkage())
      str += RD->getNameAsString();
    else
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    return str;
  }
  return "undefinedtype";
}

std::set<std::string> FindAllExternalObjects(EOObject obj) {
  std::set<std::string> all_known = {obj.postfix};
  std::set<std::string> unknown = {};
  //TODO maybe should use pointers or copy constructor to avoid unnecessary copying of objects
  std::queue<EOObject> not_visited ;
  for(auto child : obj.nested)
  {
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty())
  {
    EOObject cur = not_visited.front();
    not_visited.pop();
    switch (cur.type) {
      case EOObjectType::EO_ABSTRACT:
        all_known.insert(cur.postfix);
        break;
      case EOObjectType::EO_COMPLETE:
        all_known.insert(cur.postfix);
        if (all_known.find(cur.name)==all_known.end())
          unknown.insert(cur.name);
        break;
      case EOObjectType::EO_EMPTY: break;
      case EOObjectType::EO_LITERAL: break;
    }
    for(auto child : cur.nested)
    {
      not_visited.push(std::move(child));
    }
  }
  for (const auto& known_obj : all_known) {
    unknown.erase(known_obj);
  }

  return unknown;
}

