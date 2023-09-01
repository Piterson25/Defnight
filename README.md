
# Defnight

🎮 2D RPG PvE game made in C++ SFML.

<p align="center">
    <img src="https://github.com/Piterson25/Defnight/blob/v0.2.4/assets/textures/title.png">
</p>

#
| ![screenshot1.png](https://github.com/Piterson25/Defnight/blob/v0.2.4/screenshots/screenshot1.png) | ![screenshot2.png](https://github.com/Piterson25/Defnight/blob/v0.2.4/screenshots/screenshot2.png) |
| ---------------------------- | ---------------------------- |
| ![screenshot3.png](https://github.com/Piterson25/Defnight/blob/v0.2.4/screenshots/screenshot3.png) | ![screenshot4.png](https://github.com/Piterson25/Defnight/blob/v0.2.4/screenshots/screenshot4.png) |

Defnight is a simple single-player mob arena game, with a pixel art style. Your objective is to survive for as long as you can. Before the start of the game, you can choose a map, character, and difficulty (more characters coming in the future). The game is split into waves of monsters, with different types of monsters and difficulties. Every 10th wave is a boss fight. When you reach a new level, you can choose which attribute to level up. At levels 5 and 10, you can upgrade to a new class with abilities. Gold you've collected can be spent in the shop or upgrading abilities.

## 🛠️ Technologies used

* C++20
* CMake
* SFML
* Crypto++

## 🚀 Getting Started
To play Defnight you can:
1. Download and play it here 👉 [ITCH.IO](https://piterson25.itch.io/defnight)
2. Build it yourself (all libs are included in this repo, make sure you have CMake and C++20 compiler):
   ```sh
    cmake -S . -B build
   ```
   ```sh
    cmake --build build --config Release -j
   ```

## ⌨️ Keybinds
- Movement - **WSAD**
- Attack - **LMB**
- Ability - **RMB**
- Sprint - **LShift**
- Return - **Esc**
- Shop - **Q**
- Ability Upgrade - **E**

## 🎉 Features
### Music and sounds
Game features several sounds and dynamic music during gameplay.

### Rank System
With collecting XP during gameplay, you can gain new ranks to showcase your skills. Ranks page can be visited in left corner of main menu.

### Statistics
Your history of waves and monsters killed is collected and stored in encrypted ```.dat``` file.

### Settings
In settings you can change window properties, fps limit, audio volume and language (English/Polish).

## License
Defnight is licensed under the GPL License. See [LICENSE](https://github.com/Piterson25/Defnight/blob/v0.2.4/LICENSE.md) for more information.
