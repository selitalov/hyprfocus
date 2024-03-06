#include "Flash.hpp"
#include "Globals.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

void CFlash::init(HANDLE pHandle, std::string animationName) {
  IFocusAnimation::init(pHandle, animationName);
  addConfigValue(pHandle, "flash_opacity", Hyprlang::FLOAT{0.5f});
}

void CFlash::setup(HANDLE pHandle, std::string animationName) {
  // static const auto *flash_opacity =
  //     (Hyprlang::FLOAT *const *)(getConfigValue(pHandle, "flash_opacity")
  //                                    ->getDataStaticPtr());
  // g_fFlashOpacity = **flash_opacity;
  // hyprfocus_log(LOG, "Flash opacity: {}", g_fFlashOpacity);
  // static const auto *active_opacity =
  //     (Hyprlang::FLOAT *const *)(HyprlandAPI::getConfigValue(
  //                                    pHandle, "decoration:active_opacity")
  //                                    ->getDataStaticPtr());
  // g_fActiveOpacity = **active_opacity;
  // hyprfocus_log(LOG, "Active opacity: {}", g_fActiveOpacity);
}

void CFlash::onWindowFocus(CWindow *pWindow, HANDLE pHandle) {
  hyprfocus_log(LOG, "Flash onWindowFocus start");
  IFocusAnimation::onWindowFocus(pWindow, pHandle);

  static const auto *flash_opacity =
      (Hyprlang::FLOAT *const *)(getConfigValue(pHandle, "flash_opacity")
                                     ->getDataStaticPtr());
  pWindow->m_fAlpha = **flash_opacity;
  // pWindow->m_fAlpha = g_fFlashOpacity;
  pWindow->m_fAlpha.setConfig(&m_sFocusInAnimConfig);
  pWindow->m_fAlpha.setCallbackOnEnd([this, pWindow, pHandle](void *) {
    static const auto *active_opacity =
        (Hyprlang::FLOAT *const *)(HyprlandAPI::getConfigValue(
                                       pHandle, "decoration:active_opacity")
                                       ->getDataStaticPtr());
    // Make sure we restore to the active window opacity
    pWindow->m_fAlpha = **active_opacity;
    // pWindow->m_fAlpha = g_fActiveOpacity;
    pWindow->m_fAlpha.setConfig(&m_sFocusOutAnimConfig);
  });
}
