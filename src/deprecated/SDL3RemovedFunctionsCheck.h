#ifndef SDL2TO3_DEPRECATED_SDL3REMOVEDFUNCTIONS_CHECK_H
#define SDL2TO3_DEPRECATED_SDL3REMOVEDFUNCTIONS_CHECK_H

#include <clang-tidy/ClangTidyCheck.h>

using namespace clang;
using namespace clang::tidy;
using namespace llvm;

namespace sdl2to3::tidy::deprecated {

/// Warns when deprecated SDL2 functions: E.g:
/// * SDL_CalculateGammaRamp()
/// * SDL_GetRevisionNumber()
/// * SDL_GetWindowBrightness()
/// * SDL_GetWindowGammaRamp()
/// * SDL_RWFromFP()
/// * SDL_SetWindowBrightness()
/// * SDL_SetWindowGammaRamp()


class SDL3RemovedFunctionsCheck : public ClangTidyCheck {
public:
    SDL3RemovedFunctionsCheck(StringRef Name, ClangTidyContext *Context);

    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace sdl2to3::tidy::deprecated

#endif // SDL2TO3_DEPRECATED_SDL3REMOVEDFUNCTIONS_CHECK_H
