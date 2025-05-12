/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Structure représentant un chronomètre.
typedef struct Timer
{
    /// @brief Temps de départ.
    /// Exprimé en millisecondes.
    Uint64 m_startTime;

    /// @brief Temps du dernier appel à Timer_update().
    /// Exprimé en millisecondes.
    Uint64 m_currentTime;

    /// @brief Temps de l'avant dernier appel à Timer_update().
    /// Exprimé en millisecondes.
    Uint64 m_previousTime;

    /// @brief Ecart entre les deux derniers appels à Timer_update().
    /// Ce membre est affecté par le facteur d'échelle.
    /// Exprimé en millisecondes.
    Uint64 m_delta;

    /// @brief Ecart entre les deux derniers appels à Timer_update().
    /// Ce membre n'est pas affecté par le facteur d'échelle.
    /// Exprimé en millisecondes.
    Uint64 m_unscaledDelta;

    /// @brief Facteur d'échelle appliqué au temps.
    float m_scale;

    /// @brief Ecart de temps maximum entre deux appels à Timer_update().
    /// Exprimé en millisecondes.
    Uint64 m_maxDelta;

    /// @brief Ecart entre le lancement du timer Timer_start()
    /// et le dernier appel à Timer_update().
    /// Ce membre est affecté par le facteur d'échelle.
    /// Exprimé en millisecondes.
    Uint64 m_elapsed;

    /// @brief Ecart entre le lancement du timer Timer_start()
    /// et le dernier appel à Timer_update().
    /// Ce membre n'est pas affecté par le facteur d'échelle.
    /// Exprimé en millisecondes.
    Uint64 m_unscaledElapsed;
} Timer;

/// @brief Crée un nouveau timer.
/// @return Le timer créé.
Timer* Timer_create();

/// @brief Détruit un timer.
/// @param self le timer à détruire.
void Timer_destroy(Timer* self);

/// @brief Lance le timer.
/// @param self le timer.
void Timer_start(Timer* self);

/// @brief Met à jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appelée à chaque tour de la boucle de rendu.
/// @param self le timer.
void Timer_update(Timer* self);

/// @brief Définit le facteur d'échelle de temps appliqué à un timer.
/// Si l'échelle vaut 0.5f, le temps s'écoule deux fois moins rapidement.
/// @param self le timer.
/// @param scale le facteur d'échelle.
INLINE void Timer_setTimeScale(Timer *self, float scale)
{
    assert(self && "The Timer must be created");
    assert(scale >= 0.0f);
    self->m_scale = scale;
}

/// @brief Renvoie le facteur d'échelle de temps d'un timer.
/// @param self le timer.
/// @return Le facteur d'échelle de temps d'un timer.
INLINE float Timer_getTimeScale(Timer *self)
{
    assert(self && "The Timer must be created");
    return self->m_scale;
}

/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers
/// appels à la fonction Timer_update().
/// @param self le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
INLINE float Timer_getDelta(Timer *self)
{
    assert(self && "The Timer must be created");
    return (float)(self->m_delta) / 1000.f;
}

/// @brief Renvoie l'écart de temps (en millisecondes) entre les deux derniers
/// appels à la fonction Timer_update().
/// @param self le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
INLINE Uint64 Timer_getDeltaMS(Timer *self)
{
    assert(self && "The Timer must be created");
    return self->m_delta;
}

/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers
/// appels à la fonction Timer_update().
/// Cette méthode ne tient pas compte de l'échelle de temps du timer.
/// @param self le timer.
/// @return L'écart de temps entre les deux dernières mises à jour
/// (sans échelle de temps).
INLINE float Timer_getUnscaledDelta(Timer *self)
{
    assert(self && "The Timer must be created");
    return (float)(self->m_unscaledDelta) / 1000.f;
}

/// @brief Renvoie l'écart de temps (en millisecondes) entre les deux derniers
/// appels à la fonction Timer_update().
/// Cette méthode ne tient pas compte de l'échelle de temps du timer.
/// @param self le timer.
/// @return L'écart de temps entre les deux dernières mises à jour
/// (sans échelle de temps).
INLINE Uint64 Timer_getUnscaledDeltaMS(Timer *self)
{
    assert(self && "The Timer must be created");
    return self->m_unscaledDelta;
}

/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer
/// avec Timer_start() et le dernier appel à Timer_update().
/// @param self le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la dernière mise à jour.
INLINE float Timer_getElapsed(Timer *self)
{
    assert(self && "The Timer must be created");
    return (float)(self->m_elapsed) / 1000.f;
}

/// @brief Renvoie l'écart de temps (en millisecondes) entre le lancement du timer
/// avec Timer_start() et le dernier appel à Timer_update().
/// @param self le timer.
/// @return Le nombre de millisecondes écoulées depuis le lancement du timer et la dernière mise à jour.
INLINE Uint64 Timer_getElapsedMS(Timer *self)
{
    assert(self && "The Timer must be created");
    return self->m_elapsed;
}

/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer
/// avec Timer_start() et le dernier appel à Timer_update().
/// Cette méthode en tient pas compte de l'échelle de temps du timer.
/// @param self le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la
/// dernière mise à jour (sans échelle de temps).
INLINE float Timer_getUnscaledElapsed(Timer *self)
{
    assert(self && "The Timer must be created");
    return (float)(self->m_unscaledElapsed) / 1000.f;
}

/// @brief Renvoie l'écart de temps (en millisecondes) entre le lancement du timer
/// avec Timer_start() et le dernier appel à Timer_update().
/// Cette méthode en tient pas compte de l'échelle de temps du timer.
/// @param self le timer.
/// @return Le nombre de millisecondes écoulées depuis le lancement du timer et la
/// dernière mise à jour (sans échelle de temps).
INLINE Uint64 Timer_getUnscaledElapsedMS(Timer *self)
{
    assert(self && "The Timer must be created");
    return self->m_unscaledElapsed;
}
