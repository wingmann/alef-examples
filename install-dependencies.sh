echo Installing dependencies...

if [ ! -d "./lib" ]; then
    mkdir -p "lib"
fi

cd "./lib"

if [ ! -d "./alef-core" ]; then
    git clone https://github.com/wingmann/alef-core.git
else
    echo alef was already installed
fi

if [ ! -d "./julianah" ]; then
    git clone https://github.com/wingmann/julianah.git
else
    echo alef was already installed
fi
