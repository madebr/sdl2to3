#ifndef SDL2TO3_DEPRECATED_REMOVED_SDL_CREATERGBSURFACEFROMFORMAT_CHECK_H
#define SDL2TO3_DEPRECATED_REMOVED_SDL_CREATERGBSURFACEFROMFORMAT_CHECK_H

#include <clang-tidy/ClangTidyCheck.h>

using namespace clang;
using namespace clang::tidy;
using namespace llvm;

namespace sdl2to3::tidy::deprecated {

/// Warns when using renamed SDL2 functions: SDL_CreateRGBSurfaceFrom()


class SDL3RemovedSDL_CreateRGBSurfaceFromCheck : public ClangTidyCheck {
public:
    SDL3RemovedSDL_CreateRGBSurfaceFromCheck(StringRef Name, ClangTidyContext *Context);

    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace sdl2to3::tidy::deprecated

#endif // SDL2TO3_DEPRECATED_REMOVED_SDL_CREATERGBSURFACEFROMFORMAT_CHECK_H
