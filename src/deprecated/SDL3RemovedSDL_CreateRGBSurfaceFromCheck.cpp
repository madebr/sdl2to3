#include "SDL3RemovedSDL_CreateRGBSurfaceFromCheck.h"
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace clang::ast_matchers;

namespace sdl2to3::tidy::deprecated {

SDL3RemovedSDL_CreateRGBSurfaceFromCheck::SDL3RemovedSDL_CreateRGBSurfaceFromCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}
void SDL3RemovedSDL_CreateRGBSurfaceFromCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurfaceFrom")))).bind("call_expr"), this);
}

#include <stdio.h>
void SDL3RemovedSDL_CreateRGBSurfaceFromCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *matchExpr = Result.Nodes.getNodeAs<CallExpr>("call_expr");
    if (matchExpr->getNumArgs() != 9) {
        // SDL2 style SDL_CreateRGBSurfaceFrom expects 9 arguments
        return;
    }
    const Expr *const*args = matchExpr->getArgs();

    std::vector<std::string> argStrings;
    argStrings.reserve(matchExpr->getNumArgs());
    for (auto i = 0; i < matchExpr->getNumArgs(); i++) {
        argStrings.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(args[i]->getSourceRange()),
                                                  *Result.SourceManager, Result.Context->getLangOpts()).str());
    }
    auto repl = std::string("SDL_CreateSurfaceFrom(") + argStrings[0] + ", " + argStrings[1] + ", " + argStrings[2] + ", " + argStrings[4] +
            ", SDL_MasksToPixelFormatEnum(" + argStrings[3] + ", " + argStrings[5] + ", " + argStrings[6] + ", " + argStrings[7] + ", " + argStrings[8] + "))";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurfaceFrom is removed from SDL3, and can be replaced with SDL_CreateSurfaceFrom.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
