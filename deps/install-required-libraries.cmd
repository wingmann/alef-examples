@echo off
echo Installing dependencies...

if not exist ".\lib" (
    cd ".."
    mkdir "lib"
    
)

cd ".\lib"

if not exist ".\lib\alef-core" (
    git clone https://github.com/wingmann/alef-core.git
) else (
    echo alef was already installed
)

if not exist ".\lib\julianah" (
    git clone https://github.com/wingmann/julianah.git
) else (
    echo julianah was already installed
)
