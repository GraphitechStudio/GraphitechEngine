# `ECS::Scene` — Fonctions publiques

**Référence** : `Graphitech/ECS/ECS.hpp`

**Navigation**: [Accueil](../index.md) · [ECS header](Graphitech.ECS.ECS.md) · [Composants](Components.md)

---

## `getEntitiesWith<Components...>()`

**Signature**

```cpp
template<typename... Components>
std::vector<Entity> getEntitiesWith();
```

**Description**

Retourne la liste des entités possédant tous les composants listés dans les paramètres de template.

**Exemple**

```cpp
#include <Graphitech/ECS/ECS.hpp>

// Récupère les entités ayant une ligne à dessiner
auto lines = scene.getEntitiesWith<ECS::RenderLine>();
for (auto e : lines) {
    auto *ln = scene.getComponent<ECS::RenderLine>(e);
    if(!ln) continue;
    // utiliser ln->{x1,y1,x2,y2,r,g,b,width}
}
```

**Notes**

- L'ordre des composants n'importe pas.
- Si aucune entité ne correspond, la fonction retourne un vecteur vide.

---

## `getComponent<T>(Entity e)`

**Signature**

```cpp
template<typename T>
T* getComponent(Entity e);
```

**Description**

Retourne un pointeur vers le composant `T` attaché à l'entité `e`. Retourne `nullptr` si l'entité ne possède pas ce composant.

**Exemple**

```cpp
auto *p = scene.getComponent<ECS::PositionComponent>(entity);
if (p) {
    // lecture/modification de p->x et p->y
}
```

**Notes**

- Le pointeur renvoyé est détenu par la Scene ; ne pas le delete.
- Les modifications sur le composant modifient directement l'état stocké dans la Scene.

---

## `getMapValue<T>(const std::string &mapName, const std::string &key)`

**Signature**

```cpp
template<typename T>
T* getMapValue(const std::string &mapName, const std::string &key);
```

**Description**

Récupère une valeur stockée dans une table (map) associée à la scène. Utile pour transmettre des collections (ex : tableau d'`UIItem`) au renderer sans créer de composants.

**Exemple**

```cpp
// Supposons que la map courante contient une clé "ui" -> std::vector<ECS::UIItem>
std::string m = scene.getCurrentMap();
auto *ui = scene.getMapValue<std::vector<ECS::UIItem>>(m, "ui");
if (ui) {
    for (auto &it : *ui) {
        // it.text, it.x, it.y, it.r, it.g, it.b
    }
}
```

**Notes**

- Retourne `nullptr` si la clé ou la map n'existe pas ou si la conversion de type échoue.

---

## `getCurrentMap()`

**Signature**

```cpp
std::string getCurrentMap() const;
```

**Description**

Retourne le nom de la map courante associée à la scène (chaîne vide si aucune).

**Exemple**

```cpp
std::string cur = scene.getCurrentMap();
if(!cur.empty()) {
    auto *ui = scene.getMapValue<std::vector<ECS::UIItem>>(cur, "ui");
    // ...
}
```

**Notes**

- Utilisée principalement pour récupérer des données liées à l'UI depuis des systèmes externes (renderer, debug UI).
