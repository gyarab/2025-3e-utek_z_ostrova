> **CZ**
# Útěk z ostrova
Útěk z ostrova je singleplayerová hra typu plošinovka.
Hra je vytvořena ve 2D prostředí a využívá pixelovou grafiku.
Hra obsahuje jednu úroveň, jejíž obtížnost se postupně zvyšuje, čím dál se hráč dostane.
Cílem hráče je dostat se ze začátku úrovně až do cíle a překonat všechny překážky, které se na cestě objeví.
Úroveň se bude postupně ztěžovat – nové překážky a herní mechaniky se budou objevovat, jakmile hráč postupuje dál.
Hráč si tak bude muset osvojit logiku hry, zapamatovat si rozmístění překážek a najít způsob, jak úroveň úspěšně projít. 

### Instalace a spuštění hry
1. Stáhněte si archiv "escape_from_the_island_1_2_2_6.zip" z [releasu v1.2.2.6](https://github.com/gyarab/2025-3e-utek_z_ostrova/releases/tag/v1.2.2.6).
2. Poté ho extrahujte to jakékoli složky a nechte konfigurační soubory, .dll a další adresáře tak, jak jsou.
3. Spusťtě "escape_from_the_island.exe".
4. V případě problému by se v adresáři "logs" měl daný problém zalogovat [*ne vše je 100 % ošetřeno*].

### Vlastní kompilace hry - MS Visual Studio 2022, MSVC compiler
1. Stáhněte si zdrojové kódy v .zip archivu z [releasu v1.2.2.6](https://github.com/gyarab/2025-3e-utek_z_ostrova/releases/tag/v1.2.2.6).
2. Alternativně si můžete vyklonovat repozitář do vašeho projektu ve VS 2022.
3. Stáhněte si z github repozitáře oficiální verzi [SDL knihovny](https://github.com/libsdl-org/SDL/releases) & [SDL_image knihovny](https://github.com/libsdl-org/SDL_image/releases), kterou tento project momentálně používá.
4. Všechno by se mělo automaticky nastavit, pokud ve VS 2022 otevřete soubor řešení "Game.sln" a vše ostatní necháte tam, kde je.
5. Případně bude nutné v daném řešení otevřít všechny soubory projektů hry ["Game.vcxproj"] a knihoven ["SDL.vcxproj", "SDL_image.vcxproj"] a manuálně je prolinkovat.
6. Kompilujte jako sestavení "release, x64".
7. Pokud něco přitom nastavování nebude fungovat, rád se vám pokusím pomoc to zprovoznit.

<br>

> ### Momentální verze SDL knihoven
> - **SDL3** -> [3.2.26](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.26)
> - **SDL3_image** -> [3.2.4](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)

<br>
<br>

> **EN-US**
# Escape from the Island
Escape from the Island is a single-player platformer game.
It is created in a 2D environment and uses pixel graphics.
The game contains one level, whose difficulty gradually increases as the player progresses.
The player's goal is to reach the end of the level from the starting point and overcome all the obstacles that appear along the way.
The level becomes increasingly challenging – new obstacles and game mechanics appear as the player advances.
The player must learn the game’s logic, remember the placement of obstacles, and find a way to complete the level successfully. 

### Installing and Running the Game
1. Download the archive "escape_from_the_island_1_2_2_6.zip" from the [v1.2.2.6 release](https://github.com/gyarab/2025-3e-utek_z_ostrova/releases/tag/v1.2.2.6).
2. Then extract it to any folder and leave the configuration files, .dll files, and other directories where they are.
3. Run "escape_from_the_island.exe".
4. If a problem occurs, it should be logged in the "logs" directory [*not everything is 100% covered*].

### Compiling the Game Yourself - MS Visual Studio 2022, MSVC Compiler
1. Download the source code in a .zip archive from the [v1.2.2.6 release](https://github.com/gyarab/2025-3e-utek_z_ostrova/releases/tag/v1.2.2.6).
2. Alternatively, you can clone the repository into your project in VS 2022.
3. Download the official versions of the [SDL library](https://github.com/libsdl-org/SDL/releases) & [SDL_image library](https://github.com/libsdl-org/SDL_image/releases) from the GitHub repository, which this project currently uses.
4. Everything should be set up automatically if you open the "Game.sln" solution file in VS 2022 and leave everything else as is.
5. If necessary, you will need to open all the game project files ["Game.vcxproj"] and library files ["SDL.vcxproj", "SDL_image.vcxproj"] in the solution and link them manually.
6. Compile as a "release, x64" build.
7. If something doesn’t work during setup, I’ll be happy to help you get it running.

<br>

> ### Current versions of SDL libraries
> - **SDL3** -> [3.2.26](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.26)
> - **SDL3_image** -> [3.2.4](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)

<br>
<br>