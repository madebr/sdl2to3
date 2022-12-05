#include "SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck.h"
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace clang::ast_matchers;

namespace sdl2to3::tidy::deprecated {

SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck::SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}
void SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurfaceWithFormat")))).bind("call_expr"), this);
}

#include <stdio.h>
void SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *matchExpr = Result.Nodes.getNodeAs<CallExpr>("call_expr");
    if (matchExpr->getNumArgs() != 5) {
        // SDL2 style SDL_CreateRGBSurfaceWithFormat expects 5 arguments
        return;
    }
    const Expr *const*args = matchExpr->getArgs();

    std::vector<std::string> argStrings;
    argStrings.reserve(matchExpr->getNumArgs());
    for (auto i = 0; i < matchExpr->getNumArgs(); i++) {
        argStrings.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(args[i]->getSourceRange()),
                                                  *Result.SourceManager, Result.Context->getLangOpts()).str());
    }
    auto repl = std::string("SDL_CreateSurface(") + argStrings[1] + ", " + argStrings[2] + ", " + argStrings[4] + ")";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurfaceWithFormat is removed from SDL3, and can be replaced with SDL_CreateSurface.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
