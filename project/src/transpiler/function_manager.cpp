//
// Created by nchuykin on 28.02.2022.
//
#include <iostream>
#include "function_manager.h"
#include "transpile_helper.h"


void FunctionManager::Add(const clang::FunctionDecl *FD) {
  auto funcName = FD->getNameAsString();
  if(funcName != "main")
    funcName = "f-" + funcName;
  auto isDefinition = FD->isThisDeclarationADefinition();
  if(std::find_if(definitions.begin(), definitions.end(),
                   [&funcName](FunctionDefinition f){ return f.name == funcName; }) != definitions.end())
  {
    return;
  }
  if (isDefinition)
  {
    std::cout << "Definition of function " << funcName << " added\n";
//     auto declaration = std::remove_if(declarations.begin(), declarations.end(),
//                                       [&funcName](FunctionDeclaration f){ return f.name == funcName; });
//     declarations.erase(declaration,declarations.end());
    definitions.push_back({FD, funcName});
  }
  else
  {
    std::cout << "Declaration of function " << funcName << " added\n";
    declarations.push_back({FD, funcName});
  }

}

const std::vector<FunctionDefinition> &FunctionManager::GetAllDefinitions() {
  return definitions;
}

EOObject FunctionManager::GetFunctionCall(const clang::FunctionDecl *FD, size_t param_size) const{
  auto res_def = std::find_if(definitions.begin(), definitions.end(), [&FD] (const FunctionDefinition& decl) {
    return decl.FD == FD;
  });
  if (res_def!=definitions.end())
  {
    auto funcName = FD->getNameAsString();
    std::cout << "Definition of " << funcName << " used for function's call\n";
    EOObject call (res_def->name);
    call.prefix = "^";
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size),EOObjectType::EO_LITERAL);
    return call;
  }

  auto res_decl = std::find_if(declarations.begin(), declarations.end(), [&FD] (const FunctionDeclaration& decl){
    return decl.FD == FD;
  });
  if (res_decl!=declarations.end())
  {
    auto funcName = FD->getNameAsString();
    std::cout << "Declaration of " << funcName << " used for function's call\n";
    EOObject call (res_decl->name);
    call.prefix = "^";
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size),EOObjectType::EO_LITERAL);
    return call;
  }

  auto funcName = FD->getNameAsString();
  std::cout << "Empty object used for function's call\n";
  return EOObject(EOObjectType::EO_PLUG);
}

EOObject FunctionDefinition::GetEOObject() const {
   EOObject func_object(EOObjectType::EO_ABSTRACT);
   func_object.postfix = name;
   func_object.arguments = {"param-start","param-size"};
   func_object.nested.push_back(GetBody());
   return func_object;
}
EOObject FunctionDefinition::GetBody() const{
  return GetFunctionBody(FD);
}


