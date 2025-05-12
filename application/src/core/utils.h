/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

/// @brief Borne un entier entre une valeur minimale et une valeur maximale.
/// @param value Valeur à borner.
/// @param a Valeur minimale.
/// @param b Valeur maximale.
/// @return La valeur a si (value < a), b si (value > b) et value sinon.
INLINE int Int_clamp(int value, int a, int b)
{
    value = (value > a) ? value : a;
    value = (value < b) ? value : b;
    return value;
}

/// @brief Retourne le minimum de deux entiers.
/// @param a Premier entier.
/// @param b Second entier.
/// @return Le plus petit des deux entiers.
INLINE int Int_min(int a, int b)
{
    return (a < b) ? a : b;
}

/// @brief Retourne le maximum de deux entiers.
/// @param a Premier entier.
/// @param b Second entier.
/// @return Le plus grand des deux entiers.
INLINE int Int_max(int a, int b)
{
    return (a > b) ? a : b;
}

/// @brief Génère un entier aléatoire.
/// @param a Valeur minimale (incluse).
/// @param b Valeur maximale (exclue).
/// @return Un entier aléatoire compris entre a (inclus) et b (exclus).
INLINE int Int_randAB(int a, int b)
{
    return a + (rand() % (b - a));
}

/// @brief Génère un flottant aléatoire entre 0.f et 1.f.
/// @return Un flottant aléatoire compris entre 0.f (inclus) et 1.f (inclus).
INLINE float Float_rand01()
{
    return (float)rand() / (float)(RAND_MAX);
}

/// @brief Génère un flottant aléatoire.
/// @param a Valeur minimale.
/// @param b Valeur maximale.
/// @return Un flottant aléatoire compris entre a et b.
INLINE float Float_randAB(float a, float b)
{
    return a + (b - a) * Float_rand01();
}

/// @brief Borne un flottant entre une valeur minimale et une valeur maximale.
/// @param value Valeur à borner.
/// @param a Valeur minimale.
/// @param b Valeur maximale.
/// @return Retourne a si value < a, b si value > b, et value sinon.
INLINE float Float_clamp(float value, float a, float b)
{
    return fmaxf(a, fminf(value, b));
}

/// @brief Borne un flottant entre 0.f et 1.f.
/// @param value Valeur à borner.
/// @return Retourne 0.f si value < 0.f, 1.f si value > 1.f, et value sinon.
INLINE float Float_clamp01(float value)
{
    return Float_clamp(value, 0.0f, 1.0f);
}
