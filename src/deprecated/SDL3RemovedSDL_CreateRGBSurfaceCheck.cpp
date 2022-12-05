#include "SDL3RemovedSDL_CreateRGBSurfaceCheck.h"
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace clang::ast_matchers;

namespace sdl2to3::tidy::deprecated {

SDL3RemovedSDL_CreateRGBSurfaceCheck::SDL3RemovedSDL_CreateRGBSurfaceCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}
void SDL3RemovedSDL_CreateRGBSurfaceCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurface")))).bind("call_expr"), this);
}

#include <stdio.h>
void SDL3RemovedSDL_CreateRGBSurfaceCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *matchExpr = Result.Nodes.getNodeAs<CallExpr>("call_expr");
    if (matchExpr->getNumArgs() != 8) {
        // SDL2 style SDL_CreateRGBSurface expects 8 arguments
        return;
    }
    const Expr *const*args = matchExpr->getArgs();

    std::vector<std::string> argStrings;
    for (auto i = 0; i < matchExpr->getNumArgs(); i++) {
        argStrings.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(args[i]->getSourceRange()),
                                                     *Result.SourceManager, Result.Context->getLangOpts()).str());
    }
    // FIXME: deduce SDL_PIXELFORMAT_* from arguments when R,G,B,A are literals
    auto repl = std::string("SDL_CreateSurface(") + argStrings[1] + ", " + argStrings[2] + ", SDL_MasksToPixelFormatEnum(" + argStrings[3] + ", " + argStrings[4] + ", " + argStrings[5] + ", " + argStrings[6] +  ", " + argStrings[7] + "))";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurface is removed from SDL3, and can be replaced by SDL_CreateSurface.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
