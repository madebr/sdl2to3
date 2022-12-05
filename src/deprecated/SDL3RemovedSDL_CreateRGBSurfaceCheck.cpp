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
    Finder->addMatcher(callExpr(callee(functionDecl(hasName("SDL_CreateRGBSurface"))),
            hasArgument(3, anyOf(ignoringParenCasts(integerLiteral().bind("bppLiteral")), expr())),
            hasArgument(4, anyOf(ignoringParenCasts(integerLiteral().bind("redLiteral")), expr())),
            hasArgument(5, anyOf(ignoringParenCasts(integerLiteral().bind("grnLiteral")), expr())),
            hasArgument(6, anyOf(ignoringParenCasts(integerLiteral().bind("bluLiteral")), expr())),
            hasArgument(7, anyOf(ignoringParenCasts(integerLiteral().bind("alpLiteral")), expr()))
            ).bind("call_expr"), this);
}

const std::vector<SDL_pixelformat_info> SDL_MASKS = {
    {"SDL_PIXELFORMAT_RGBA8888", 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,  },
    {"SDL_PIXELFORMAT_ARGB8888", 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,  },
    {"SDL_PIXELFORMAT_BGRA8888", 32, 0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF,  },
    {"SDL_PIXELFORMAT_XBGR8888", 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000,  },
    {"SDL_PIXELFORMAT_ABGR8888", 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,  },
    {"SDL_PIXELFORMAT_ARGB4444", 16, 0x00000F00, 0x000000F0, 0x0000000F, 0x0000F000,  },
    {"SDL_PIXELFORMAT_XRGB4444", 16, 0x00000F00, 0x000000F0, 0x0000000F, 0x00000000,  },
    {"SDL_PIXELFORMAT_RGBA4444", 16, 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F,  },
    {"SDL_PIXELFORMAT_XBGR4444", 16, 0x0000000F, 0x000000F0, 0x00000F00, 0x00000000,  },
    {"SDL_PIXELFORMAT_XRGB1555", 16, 0x00007C00, 0x000003E0, 0x0000001F, 0x00000000,  },
    {"SDL_PIXELFORMAT_XBGR1555", 16, 0x0000001F, 0x000003E0, 0x00007C00, 0x00000000,  },
    {"SDL_PIXELFORMAT_RGB565", 16, 0x0000F800, 0x000007E0, 0x0000001F, 0x00000000,  },
    {"SDL_PIXELFORMAT_BGR565", 16, 0x0000001F, 0x000007E0, 0x0000F800, 0x00000000,  },
};

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
    auto repl = std::string("SDL_CreateSurface(") + argStrings[1] + ", " + argStrings[2] + ", " + pixelformat_str + ")";

    diag(matchExpr->getExprLoc(), "SDL_CreateRGBSurface is removed from SDL3, and can be replaced by SDL_CreateSurface.")
        << clang::FixItHint::CreateReplacement(matchExpr->getSourceRange(), repl);
}

} // namespace sdl2to3::tidy::deprecated
