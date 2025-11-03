# ⚔️ Game Combat Engineer

A **Hack-and-Slash Prototype** built in **Unreal Engine 5.5**, demonstrating a modular **combat system architecture** with combo mechanics, smooth animation transitions, and AI-driven interactions.
Developed as part of the **AtherLabs Technical Test**.

---

## 🎮 Overview

This prototype focuses on implementing a **core melee combat system** using Unreal Engine’s **Animation Montage**, **Enhanced Input**, and **Actor Component** frameworks.
The architecture emphasizes **data-driven design**, scalability, and code reusability for future extensions like **Gameplay Ability System (GAS)** or advanced combat modules.

---

## ✅ Implemented Features

### ⚔️ Combat System

* **Ground Combo Chain (3+ attacks)** — light attack chain using sectioned Montages.
* **Smooth Animation Transitions** — controlled via blend settings and montage notifies.
* **Modular Combat Logic** handled by `UAttackComponent` + `IAttackInterface`.
* **Hit Detection** via `SphereTraceMultiForObjects`, configured through `CharacterData`.
* **Debug Visualization** for trace impact points and bone hits.
* **Enemy AI Combat (Extra Feature)** — basic AI using Behavior Tree, capable of detecting and attacking the player.

---

### 🔁 Dynamic Character & Camera

* Third-person camera using **USpringArmComponent** and **UCameraComponent**.
* Smooth camera follow behavior with yaw control.
* Character **rotation aligned with movement direction**.
* **Runtime adjustable movement speed** (walk ⇄ run toggle).

---

### 🧩 Data-Driven Architecture

* `UCharacterData` — defines combat/movement parameters such as rotation rate, trace radius, and speed.
* `UInputData` — contains `InputAction` assets and mapping context.
* `UAttackComponent` — encapsulates attack logic, montage play, and trace checks.
* `IAttackInterface` — provides an abstraction layer for any actor capable of performing attacks.

---

## 🧱 Project Structure

```
GameCombatEngineer/
│
├── Source/GameCombatEngineer/
│   ├── Character/
│   │   ├── BaseCharacter.h / .cpp     ← main playable character
│   │   └── EnemyCharacter.h / .cpp    ← basic AI enemy
│   │
│   ├── Component/
│   │   └── AttackComponent.h / .cpp   ← core attack logic
│   │
│   ├── DataAsset/
│   │   ├── InputData.h / .cpp         ← input mapping context
│   │   └── CharacterData.h / .cpp     ← movement/combat config
│   │
│   ├── Interface/
│   │   └── AttackInterface.h          ← interface for combat interaction
│   │
│   ├── AI/
│   │   ├── EnemyAIController.h / .cpp
│   │   └── BehaviorTree/
│   │       ├── BT_EnemyBehavior
│   │       └── Tasks/
│   │
│   └── GameCombatEngineer.Build.cs
│
└── Content/
    ├── Animations/
    ├── Montages/
    ├── Blueprints/
    ├── DataAssets/
    └── AI/
```

---

## ⚙️ Technical Highlights

| Category             | Description                   |
| -------------------- | ----------------------------- |
| **Engine**           | Unreal Engine 5.5             |
| **Language**         | C++                           |
| **Input System**     | Enhanced Input (UE5)          |
| **Animation System** | Animation Montages + Notifies |
| **AI**               | Behavior Tree + AIController  |
| **Target Platform**  | PC                            |

---

## 🧪 Current Progress

| Feature                     | Status      | Notes                                        |
| --------------------------- | ----------- | -------------------------------------------- |
| Ground Combo Chain (3+)     | ✅ Completed | 3 attack chain with smooth blend transitions |
| Smooth Animation Transition | ✅ Completed | Section blending & notifies                  |
| Enemy AI                    | ✅ Completed | Basic pursuit and attack                     |
| Air Combo Chain (2+)        | 🚧 Planned  | Will extend from existing montage system     |
| HUD (HP/Stamina/Combo)      | 🚧 Planned  | Part of UI phase                             |
| Camera Shake & Collision    | 🚧 Planned  | In next iteration                            |
| GAS Integration             | 🚧 Planned  | Future implementation                        |

---

## 🧠 Design Diagram
```mermaid
flowchart TD
    A[Player Input (Enhanced Input)] --> B[UAttackComponent]
    B --> C[IAttackInterface::I_PlayAttackMontage()]
    C --> D[Animation Montage / Notifies]
    D --> E[UAttackComponent::TraceHit()]
    E --> F[SphereTraceMultiForObjects]
    F --> G[HitResult / Apply Damage]
    G --> H[Enemy AI Response]
```

---

## 📹 Demo

🎥 **YouTube Link:** *Coming Soon*
*(Showcasing ground combo chain, smooth transition, and enemy AI behavior)*

---

## 🧩 Next Steps

* Implement **Air Combo System (2+ attacks)**
* Add **Stamina system & attack cost**
* Integrate **Camera Shake** and **Impact feedback**
* Introduce **DoT / Poison effect** system
* Expand **HUD** for HP, Stamina, and Combo display

---

## 📜 License

This project was developed for **educational and testing purposes** under AtherLabs’ Technical Assessment.
All used assets are from **Unreal Engine samples** or custom-created placeholders.

---

## 👤 Author

**Thái Đức Lợi**
Gameplay Programmer | Combat System Designer
📧 [thaiducloi2000@gmail.com](mailto:thaiducloi2000@gmail.com)
🔗 [GitHub: thaiducloi2000](https://github.com/thaiducloi2000)
