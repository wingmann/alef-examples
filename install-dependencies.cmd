@echo off
echo Installing dependencies...

if not exist ".\lib" (
    mkdir "lib"
)

cd ".\lib"

if not exist ".\alef" (
    git clone https://github.com/wingmann/alef.git
) else (
    echo alef was already installed
)

if not exist ".\biginteger" (
    git clone https://github.com/wingmann/biginteger.git
) else (
    echo biginteger was already installed
)

if not exist ".\cast" (
    git clone https://github.com/wingmann/cast.git
) else (
    echo cast was already installed
)

if not exist ".\concepts" (
    git clone https://github.com/wingmann/concepts.git
) else (
    echo concepts was already installed
)

if not exist ".\containers" (
    git clone https://github.com/wingmann/containers.git
) else (
    echo containers was already installed
)

if not exist ".\err-fusion" (
    git clone https://github.com/wingmann/err-fusion.git
) else (
    echo err-fusion was already installed
)

if not exist ".\iterators" (
    git clone https://github.com/wingmann/iterators.git
) else (
    echo iterators was already installed
)

if not exist ".\julianah" (
    git clone https://github.com/wingmann/julianah.git
) else (
    echo julianah was already installed
)
