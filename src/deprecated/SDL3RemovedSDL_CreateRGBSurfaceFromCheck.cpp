#include "SDL3RemovedSDL_CreateRGBSurfaceFromCheck.h"
#include "SDL3RemovedSDL_CreateRGBSurfaceCheck.h"
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
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurfaceFrom"))),
            hasArgument(3, anyOf(ignoringParenCasts(integerLiteral().bind("bppLiteral")), expr())),
            hasArgument(5, anyOf(ignoringParenCasts(integerLiteral().bind("redLiteral")), expr())),
            hasArgument(6, anyOf(ignoringParenCasts(integerLiteral().bind("grnLiteral")), expr())),
            hasArgument(7, anyOf(ignoringParenCasts(integerLiteral().bind("bluLiteral")), expr())),
            hasArgument(8, anyOf(ignoringParenCasts(integerLiteral().bind("alpLiteral")), expr()))
            ).bind("call_expr"), this);
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
    for (auto i = 0; i < matchExpr->getNumArgs(); i++) {
        argStrings.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(args[i]->getSourceRange()),
                                                  *Result.SourceManager, Result.Context->getLangOpts()).str());
    }
    const auto *bppLiteralExpr = Result.Nodes.getNodeAs<IntegerLiteral>("bppLiteral");
    const auto *redLiteralExpr = Result.Nodes.getNodeAs<IntegerLiteral>("redLiteral");
    const auto *grnLiteralExpr = Result.Nodes.getNodeAs<IntegerLiteral>("grnLiteral");
    const auto *bluLiteralExpr = Result.Nodes.getNodeAs<IntegerLiteral>("bluLiteral");
    const auto *alpLiteralExpr = Result.Nodes.getNodeAs<IntegerLiteral>("alpLiteral");

    std::string pixelformat_str = std::string("SDL_MasksToPixelFormatEnum(") + argStrings[3] + ", " + argStrings[4] + ", " + argStrings[5] + ", " + argStrings[6] +  ", " + argStrings[7] + ")";
    if (bppLiteralExpr && redLiteralExpr && grnLiteralExpr && bluLiteralExpr && alpLiteralExpr) {
        uint32_t bpp = (uint32_t)bppLiteralExpr->getValue().getZExtValue();
        uint32_t red = (uint32_t)redLiteralExpr->getValue().getZExtValue();
        uint32_t grn = (uint32_t)grnLiteralExpr->getValue().getZExtValue();
        uint32_t blu = (uint32_t)bluLiteralExpr->getValue().getZExtValue();
        uint32_t alp = (uint32_t)alpLiteralExpr->getValue().getZExtValue();
        for (const auto &sdl_format: SDL_MASKS) {
            if (bpp == sdl_format.bpp && red == sdl_format.red && grn == sdl_format.grn && blu == sdl_format.blu && alp == sdl_format.alp) {
                pixelformat_str = sdl_format.str;
                break;
            }
        }
    }

    auto repl = std::string("SDL_CreateSurfaceFrom(") + argStrings[0] + ", " + argStrings[1] + ", " + argStrings[2] + ", " + argStrings[4] +
            ", " + pixelformat_str + ")";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurfaceFrom is removed from SDL3, and can be replaced with SDL_CreateSurfaceFrom.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
