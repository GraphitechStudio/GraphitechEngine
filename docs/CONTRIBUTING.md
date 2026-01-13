# Contribuer au développement de l'ECS

**Navigation**: [Accueil](index.md) · [RFC](rfc/0001-ecs.md) · [API](api/Graphitech.ECS.ECS.md)

Ce document explique comment contribuer au code et à la documentation de l'ECS.

## Pré-requis

- Compiler le projet depuis la racine avec CMake :
  - mkdir build && cd build && cmake .. && make

## Processus de contribution

1. Créez une branche `feature/` ou `fix/` à partir de `main`.
2. Faites vos modifications et ajoutez des tests si nécessaire.
3. Documentez toute API publique et chaque nouveau fichier d'en-tête public :
   - **Un fichier Markdown par `.hpp` public** dans `docs/api/`, nommé `<Namespace>.<Header>.md` (ex. : `Graphitech.ECS.ECS.md`).
   - Ne copiez jamais le `.hpp` : la documentation doit être en Markdown, complète et autonome (signatures, description, paramètres, valeur de retour, exemples d'utilisation).
   - Ajoutez la page dans `docs/SUMMARY.md` pour qu'elle apparaisse dans le menu latéral.
4. Ouvrez une Pull Request décrivant les changements et les exemples d'utilisation.
5. Assurez-vous que la PR inclut : build qui passe, tests (si applicables), et documentation mise à jour.

## Format des pages API

Chaque page d'API doit contenir :

- Titre et référence au header (chemin relatif vers le fichier `.hpp` dans le repo si disponible).
- Pour chaque fonction :
  - Signature (ex. : `template<typename T> T* getComponent(Entity e);`)
  - Brève description
  - Paramètres et valeur de retour
  - Exemple d'utilisation complet et autonome (copiable dans un petit test)
  - Notes sur les invariants (ex. : retourne `nullptr` si absent)

## Tests

- Lancer les tests unitaires après `make` (si présents).

## Style et règles

- Ne pas utiliser d'`emoji` dans la documentation.
- Pas de fichiers `.hpp` dans `docs/` (interdit explicitement).
- Garder les descriptions concises et en français.

## Questions et revue

- Pour des modifications importantes, créez d'abord une RFC (sous `docs/rfc/`) afin de lancer la discussion.
