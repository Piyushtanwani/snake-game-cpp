#include "SoundManager.h"

SoundManager::SoundManager() {
}

void SoundManager::PlayEatSound() const {
    // Simple, clean eating sound - just one satisfying beep
    Beep(800, 100);
}

void SoundManager::PlayMenuSelectSound() const {
    // Light tone for menu selection
    Beep(600, 80);
}