#ifndef SDL2TO3_SDL3SUBFOLDERINCLUDECHECK_H
#define SDL2TO3_SDL3SUBFOLDERINCLUDECHECK_H

#include <clang-tidy/ClangTidyCheck.h>

using namespace clang;
using namespace clang::tidy;
using namespace llvm;

namespace sdl2to3::tidy::includes {

/// Warns when old-style SDL2 includes are detected. E.g:
///
///     #include "SDL.h"            // warning
///     #include <SDL2/SDL.h>       // warning
///     #include "SDL_image.h"      // warning
///     #include "my_project.h"     // no diagnostic


class SDL3SubfolderIncludeCheck : public ClangTidyCheck {
public:
    SDL3SubfolderIncludeCheck(StringRef Name, ClangTidyContext *Context);
    void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                             Preprocessor *ModuleExpanderPP) override;
};

} // namespace sdl2to3::tidy::includes

#endif // SDL2TO3_SDL3SUBFOLDERINCLUDECHECK_H
