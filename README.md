# Transformation Visualizer
Program written in C++ to visualize 2 dimensional transformation with command based interface.

## Usage
- Download the program in Releases tab
- Extract it in a folder
- Run `main.exe`

## Building
1. Clone repository
2. Install dependencies
3. Run `g++ -std=c++11 -I ./src/include -L ./src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2`
4. (Optional) Create Makefile and add the following
```make
all:
	g++ -std=c++11 -I ./src/include -L ./src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 
```

### Dependencies
- [SDL](https://github.com/libsdl-org/SDL)
- [json](https://github.com/nlohmann/json)

### Installing Dependencies
1. Download [JSON library](https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp)
2. Put it in the working folder
3. Download [SDL library](https://github.com/libsdl-org/SDL/releases/download/release-2.26.5/SDL2-devel-2.26.5-mingw.zip) specifically for development with mingw
4. Extract `SDL2-2.26.5\x86_64-w64-mingw32\bin\SDL2.dll`
5. Create new folder `src`
6. Extract both `SDL2-2.26.5\x86_64-w64-mingw32\include` and `SDL2-2.26.5\x86_64-w64-mingw32\lib` in `src` folder
7. You should have something like this
```
├── src
│   ├── include
│   │   └── ...
│   └── lib
│       └── ...
├── json.hpp
├── main.cpp
└── ...
```

## Common Error
### SDL.dll was not found
Make sure you extracted both files in the zip. 

