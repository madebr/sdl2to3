#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang-tidy/ClangTidy.h>
#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

#include "includes/SDL3SubfolderIncludeCheck.h"
#include "deprecated/SDL3RemovedFunctionsCheck.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class MyTestCheck : public ClangTidyCheck {
public:
    MyTestCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}
};

namespace {
class CTTestModule : public ClangTidyModule {
public:
    void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
        CheckFactories.registerCheck<sdl2to3::tidy::includes::SDL3SubfolderIncludeCheck>("sdl2to3-includefolder");
        CheckFactories.registerCheck<sdl2to3::tidy::deprecated::SDL3RemovedFunctionsCheck>("sdl2to3-removedfunctions");
    }
};
} // namespace

namespace clang {
namespace tidy {

// Register the CTTestTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::CTTestModule>
        X("mytest-module", "Adds my checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the CTTestModule.
volatile int CTTestModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
