#include "SDL3RemovedFunctionsCheck.h"
#include <clang/AST/ASTContext.h>
//#include <clang/utils/ASTUtils.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>

using namespace clang::ast_matchers;

namespace sdl2to3::tidy::deprecated {

// FIXME: create new check for following functions
//    "SDL_CreateRGBSurface",
//    "SDL_CreateRGBSurfaceWithFormat",
//    "SDL_CreateRGBSurfaceFrom",
//    "SDL_CreateRGBSurfaceWithFormatFrom",

SDL3RemovedFunctionsCheck::SDL3RemovedFunctionsCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}
void SDL3RemovedFunctionsCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
    Finder->addMatcher(callExpr(callee(functionDecl(
            anyOf(
                hasName("SDL_CalculateGammaRamp"),
                hasName("SDL_GetRevisionNumber"),
                hasName("SDL_GetWindowBrightness"),
                hasName("SDL_GetWindowGammaRamp"),
                hasName("SDL_RWFromFP"),
                hasName("SDL_SetWindowBrightness"),
                hasName("SDL_SetWindowGammaRamp")
                                 )))).bind("call_expr"), this);
}

#include <stdio.h>
void SDL3RemovedFunctionsCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *matchExpr = Result.Nodes.getNodeAs<CallExpr>("call_expr");
    diag(matchExpr->getExprLoc(), "This function is removed from SDL3.")
    << clang::FixItHint::CreateRemoval(matchExpr->getSourceRange());
}

} // namespace sdl2to3::tidy::deprecated
