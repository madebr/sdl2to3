#include "SDL3SubfolderIncludeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Twine.h"
#include <clang/Basic/Diagnostic.h>

#include <unordered_set>

namespace sdl2to3::tidy::includes {

// FIXME: SDL_config.h is removed
// FIXME: SDL_opengles.h is removed
// FIXME: SDL_net.h will not survive the jump to SDL3

static const StringSet sdl2_headers = {
    "begin_code.h",
    "close_code.h",
    "SDL_assert.h",
    "SDL_atomic.h",
    "SDL_audio.h",
    "SDL_bits.h",
    "SDL_blendmode.h",
    "SDL_clipboard.h",
    "SDL_copying.h",
    "SDL_cpuinfo.h",
    "SDL_egl.h",
    "SDL_endian.h",
    "SDL_error.h",
    "SDL_events.h",
    "SDL_filesystem.h",
    "SDL_gamecontroller.h",
    "SDL_gesture.h",
    "SDL_guid.h",
    "SDL.h",
    "SDL_haptic.h",
    "SDL_hidapi.h",
    "SDL_hints.h",
    "SDL_joystick.h",
    "SDL_keyboard.h",
    "SDL_keycode.h",
    "SDL_loadso.h",
    "SDL_locale.h",
    "SDL_log.h",
    "SDL_main.h",
    "SDL_messagebox.h",
    "SDL_metal.h",
    "SDL_misc.h",
    "SDL_mouse.h",
    "SDL_mutex.h",
    "SDL_name.h",
    "SDL_opengles2_gl2ext.h",
    "SDL_opengles2_gl2.h",
    "SDL_opengles2_gl2platform.h",
    "SDL_opengles2.h",
    "SDL_opengles2_khrplatform.h",
    "SDL_opengl_glext.h",
    "SDL_opengl.h",
    "SDL_pixels.h",
    "SDL_platform.h",
    "SDL_power.h",
    "SDL_quit.h",
    "SDL_rect.h",
    "SDL_render.h",
    "SDL_revision.h",
    "SDL_rwops.h",
    "SDL_scancode.h",
    "SDL_sensor.h",
    "SDL_shape.h",
    "SDL_stdinc.h",
    "SDL_surface.h",
    "SDL_system.h",
    "SDL_syswm.h",
    "SDL_test_assert.h",
    "SDL_test_common.h",
    "SDL_test_compare.h",
    "SDL_test_crc32.h",
    "SDL_test_font.h",
    "SDL_test_fuzzer.h",
    "SDL_test.h",
    "SDL_test_harness.h",
    "SDL_test_images.h",
    "SDL_test_log.h",
    "SDL_test_md5.h",
    "SDL_test_memory.h",
    "SDL_test_random.h",
    "SDL_thread.h",
    "SDL_timer.h",
    "SDL_touch.h",
    "SDL_ttf.h",
    "SDL_types.h",
    "SDL_version.h",
    "SDL_video.h",
    "SDL_vulkan.h",
    "SDL_mixer.h",
    "SDL_image.h",
    "SDL_net.h",
};

namespace {
class SDL3SubfolderIncludePPCallbacks : public PPCallbacks {
public:
    explicit SDL3SubfolderIncludePPCallbacks(SDL3SubfolderIncludeCheck &Check,
                                          const SourceManager &SM,
                                          Preprocessor *PP)
            : Check(Check), PP(PP) {}

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange,
                            Optional<FileEntryRef> File, StringRef SearchPath,
                            StringRef RelativePath, const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override;

private:
    SDL3SubfolderIncludeCheck &Check;
    Preprocessor *PP;
};
} // namespace

SDL3SubfolderIncludeCheck::SDL3SubfolderIncludeCheck(StringRef Name,
                                               ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
}

void SDL3SubfolderIncludeCheck::registerPPCallbacks(
        const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(
            ::std::make_unique<SDL3SubfolderIncludePPCallbacks>(*this, SM, PP));
}

Optional<std::string> suggest_SDL3_include(StringRef IncludePath) {
    IncludePath = llvm::sys::path::remove_leading_dotslash(IncludePath);
    if (sdl2_headers.contains(IncludePath)) {
        return ("include <SDL3/" + Twine(IncludePath) + ">").str();
    }
    // FIXME: what about deprecated SDL2 headers?
    if (IncludePath.startswith("SDL2/")) {
        auto filename = IncludePath.substr(5);
        if (sdl2_headers.contains(filename)) {
            return ("include <SDL3/" + Twine(filename) + ">").str();
        }
    }
    return {};
}

void SDL3SubfolderIncludePPCallbacks::InclusionDirective(
        SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName,
        bool IsAngled, CharSourceRange FilenameRange, Optional<FileEntryRef> File,
        StringRef SearchPath, StringRef RelativePath, const Module *Imported,
        SrcMgr::CharacteristicKind FileType) {

    if (IncludeTok.getIdentifierInfo()->getPPKeywordID() == tok::pp_import) {
        return;
    }
    auto suggestion = suggest_SDL3_include(FileName);
    if (!suggestion) {
        return;
    }

    SourceLocation DiagLoc = FilenameRange.getBegin().getLocWithOffset(1);

    SourceRange rng(IncludeTok.getLocation(), FilenameRange.getEnd());
    Check.diag(DiagLoc, "SDL2-style file inclusion is deprecated.")
        << clang::FixItHint::CreateReplacement(rng, *suggestion);
}

} // namespace sdl2to3::tidy::includes
