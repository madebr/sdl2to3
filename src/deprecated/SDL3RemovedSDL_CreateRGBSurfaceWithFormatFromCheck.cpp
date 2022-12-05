#include "SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck.h"
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace clang::ast_matchers;

namespace sdl2to3::tidy::deprecated {

SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck::SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}
void SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurfaceWithFormatFrom")))).bind("call_expr"), this);
}

#include <stdio.h>
void SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *matchExpr = Result.Nodes.getNodeAs<CallExpr>("call_expr");
    if (matchExpr->getNumArgs() != 6) {
        // SDL2 style SDL_CreateRGBSurfaceWithFormatFrom expects 6 arguments
        return;
    }
    const Expr *const*args = matchExpr->getArgs();

    std::vector<std::string> argStrings;
    argStrings.reserve(matchExpr->getNumArgs());
    for (auto i = 0; i < matchExpr->getNumArgs(); i++) {
        argStrings.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(args[i]->getSourceRange()),
                                                  *Result.SourceManager, Result.Context->getLangOpts()).str());
    }
    auto repl = std::string("SDL_CreateSurfaceFrom(") + argStrings[0] + ", " + argStrings[1] + ", " + argStrings[2] + ", " + argStrings[4] + ", " + argStrings[5] + ")";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurfaceWithFormatFrom is removed from SDL3, and can be replaced with SDL_CreateSurfaceFrom.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
