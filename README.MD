a very lightweight game written in c

work in progress

this repository contains a lot of shitcode because i almost never worked with THIS low-level and non-object oriented languages

## tl;dr

go to the `releases` section

## congitiohazards

if you dont want to go insane i strongly advise you not to open `src/obj.c` file

## dependencies

\*windows\*

all dependencies are preinstalled with your system or provided with an exe file

\*fedora\*

you need the following packages:

```
sudo dnf install gcc glfw-devel libglvnd-devel stb-devel make python3 xxd upx
```

additional packages if you want to build cgame for windows:

```
sudo dnf install mingw32-gcc
```

## build

```
git clone https://github.com/DummrenDuck/cgame.git
cd cgame
make release
```