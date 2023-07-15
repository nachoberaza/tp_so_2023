#!/bin/bash
echo "ETAPA: COPIAR SHARED LIBRARY"
sudo cp libshared-library.so /usr/lib
sudo cp -r ../shared-library/shared /usr/include
echo "ETAPA: SHARED LIBRARY COPIADA CORRECTAMENTE"
echo "ETAPA: COPIAR COMMONS"
cd ~/so-commons-library/
make debug
make install
echo "ETAPA: COMMONS COPIADA CORRECTAMENTE"
echo "ETAPA: LIMPIEZA Y COMPILACION DE MODULOS"
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/memory-module/Debug/
make clean
make all
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/file-system-module/Debug/
make clean
make all
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/cpu-module/Debug/
make clean
make all
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/kernel-module/Debug/
make clean
make all
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/memory-module/Debug/
make clean
make all
echo "ETAPA: MODULOS COMPILADOS"
echo "ETAPA: CREAR DIRECTORIO DE FCB"
cd ~/tp-2023-1c-Compu-mundo-hiper-mega-red/file-system-module/adminStructs/fs
mkdir fcb
echo "ETAPA: DIRECTORIO VACIO DE FCB CREADO"