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

### 👁️ Dynamic Character & Camera

* Third-person camera using **USpringArmComponent** and **UCameraComponent**.
* Smooth camera follow behavior with yaw control.
* Character **rotation aligned with movement direction**.
* **Runtime adjustable movement speed** (walk ↔ run toggle).

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
├── Source/
│ └── GameCombatEngineer/
│ ├── Private/ ← Implementation (.cpp)
│ ├── Public/ ← Headers (.h)
│ │
│ │── AnimCallback/ ← Animation Notify & Montage events
│ │ ├── AN_EndHitReact.h ← Notify for hit-react end
│ │ ├── AN_OnAttackCallback.h ← Notify for attack hit events
│ │ ├── AN_State_Trace.h ← Notify for trace state transitions
│ │ └── Combo_AN.h ← Notify for combo chain control
│ │
│ │── Character/ ← Character logic & base classes
│ │ ├── BaseCharacter.h ← Base class for all characters
│ │ ├── CharacterAnimationInstance.h ← Custom anim instance
│ │ ├── EnemyCharacter.h ← Enemy implementation
│ │ └── PlayerCharacter.h ← Player implementation
│ │
│ │── Component/ ← Modular gameplay components
│ │ ├── AttackComponent.h ← Handles attack + trace logic
│ │ ├── HealthComponent.h ← Health management & damage events
│ │ └── StaminaComponent.h ← Stamina & energy system
│ │
│ │── Controller/ ← AI & player controller logic
│ │ └── EnemyAIController.h ← Basic enemy AI controller
│ │
│ │── DataAsset/ ← Configurable gameplay data
│ │ ├── CharacterData.h ← Movement & stat tuning
│ │ └── InputData.h ← Input mapping context / actions
│ │
│ │── Enum/ ← Game-specific enumerations
│ │ ├── AIState.h ← Enemy AI behavior states
│ │ ├── AttackType.h ← Attack classification (Light/Heavy)
│ │ └── CombatState.h ← Combat state tracking
│ │
│ │── Interface/ ← Interfaces for modular design
│ │ ├── AttackInterface.h ← Interface for attack logic access
│ │ └── EnemyInterface.h ← Interface for enemy behaviors
│ │
│ │── Widget/ ← UI logic and UMG bindings
│ │ └── PlayerWidget.h ← Player HUD / combo counter
│ │
│ ├── GameCombatEngineer.cpp
│ └── GameCombatEngineer.Build.cs
│
├── Config/ ← Default engine/project settings
└── Content/ ← Assets: animation, BP, AI, UI, etc.
├── Animations/
├── Montages/
├── Blueprints/
├── DataAssets/
├── AI/
└── UI/
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
| Air Combo Chain (2+)        | 🚧 Planned  | Will extend from existing montage system (and Looking for other aniamtion)|
| HUD (HP/Stamina/Combo)      | ✅ Completed | Part of UI phase                             |
| Camera Shake & Collision    | ✅ Completed  | In next iteration                            |
| GAS Integration             | 🚧 Planned  | Future implementation                        |

---

## 🧠 Design Flow

**Combat Flow:**

1. Player input triggers `UAttackComponent::RequestAttack()`.
2. `IAttackInterface::I_PlayAttackMontage()` plays the Montage from `CharacterData`.
3. Animation Notifies call `TraceHit()` for real-time collision detection.
4. `SphereTraceMultiForObjects()` detects hit actors and logs impact points.
5. AI Enemy reacts based on Behavior Tree logic.

---

## 📺 Demo

🎥 **YouTube Link:** [Watch Demo on YouTube](https://youtu.be/hj1upizdoiI?si=rgC-LJYuFpthSlGB)
*(Showcasing ground combo chain, smooth transition, and enemy AI behavior - keep updating)*

---

## 🧩 Next Steps

* Implement **Air Combo System (2+ attacks)**
* Introduce **DoT / Poison effect** system
* Expand **HUD** for Combo display

---

## 📜 License

This project was developed for **educational and testing purposes** under AtherLabs’ Technical Assessment.
All used assets are from **Unreal Engine samples** or custom-created placeholders.

---

## 👤 Author

**Thái Đức Lợi**
Gameplay Programmer
📧 [thaiducloi2000@gmail.com](mailto:thaiducloi2000@gmail.com)
🔗 [GitHub: thaiducloi2000](https://github.com/thaiducloi2000)
