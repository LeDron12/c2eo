/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"
#include "generator.h"
#include "util.h"
#include "unit_transpiler.h"


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("c2eo options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");


// void generateSpace(SpaceGen &globGen, std::string objFilename,
//                    std::string initFilename) ;
void generateSpace(SpaceGen &globGen, std::string objFilename);

const char **transform_argv(const char *const *argv);

UnitTranspiler transpiler;
//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {

    if (argc < 3) {
        llvm::errs() << "Incorrect command line format. Necessary: ./c2eo <C-file-name> item-name\n";
        return -1;
    }

    int parser_argc = 3;
    const char **parser_argv = transform_argv(argv);
    const char* inputFileName = argv[1];
    std::string filename = argv[2];

    auto ExpectedParser
            = CommonOptionsParser::create(parser_argc, parser_argv, MyToolCategory, llvm::cl::Optional);

    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());


////    Matchers matchers;
////    return Tool.run(matchers.getFrontEndActionFactory());
    LoopAnalyzer loopAnalyzer;
    MatchFinder finder;
    addMatchers(finder);
//     Finder.addMatcher(LoopMatcher, &loopAnalyzer);
// 
   auto result = Tool.run(newFrontendActionFactory(&finder).get());
   if (!result)
     return result;

   // тестовый вывод
   //std::cout << transpiler;
   std::ofstream out(filename);
   out << transpiler;
}

const char **transform_argv(const char *const *argv) {
    const char** parser_argv = new const char*[3];
    parser_argv[0] = argv[0];
    parser_argv[1] = argv[1];
    parser_argv[2] = "--";
    return parser_argv;
}


// void generateSpace(SpaceGen &globGen, std::string objFilename,
//         std::string initFilename) {
//     std::string obj;
//     std::string init;
//     globGen.Generate(obj);
//     globGen.GenValue(init);
//     outs() << "\n===================================\n";
//     outs() << obj;
//     str2file(obj, objFilename);
//     outs() << init;
//     str2file(init, initFilename);
// }

void generateSpace(SpaceGen &globGen, std::string objFilename) {
    std::stringstream obj;
    //std::string init;
    globGen.Generate(obj);
    //globGen.GenValue(init);
    outs() << "\n===================================\n";
    outs() << obj.str();
    str2file(obj.str(), objFilename);
    //outs() << init;
    //str2file(init, initFilename);
}
