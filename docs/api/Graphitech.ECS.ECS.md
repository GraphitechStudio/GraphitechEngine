# API — `Graphitech/ECS/ECS.hpp`

> Référence : `Graphitech/ECS/ECS.hpp` (référencer le header dans le repo si présent). Si le header n'est pas disponible publiquement, cette page décrit l'API publique qu'il expose.

## Contenu principal

- Type principal : `ECS::Scene` — voir la documentation détaillée : [ECS::Scene (fonctions)](Scene.md)
- Composants fréquemment utilisés : voir [Composants ECS](Components.md)

## Usage général

L'ECS expose une `Scene` qui permet de rechercher des entités par ensemble de composants et d'accéder aux composants via `getComponent<T>(Entity)`. Les valeurs mappées (UI, autres tables liées à la scène) peuvent être lues via `getMapValue<T>(mapName, key)`.

Référez-vous à `docs/api/Scene.md` pour la documentation complète des fonctions et exemples d'utilisation.
