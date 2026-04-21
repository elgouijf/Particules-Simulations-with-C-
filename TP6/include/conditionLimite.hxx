#ifndef CONDITION_LIMITE_HXX
#define CONDITION_LIMITE_HXX

/**
 * @brief Types de conditions aux limites applicables sur les bords du domaine.
 */
enum class ConditionLimite {
    Reflexive,
    Absorbante,
    Periodique
};

#endif // CONDITION_LIMITE_HXX