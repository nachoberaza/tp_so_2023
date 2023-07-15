#!/bin/bash
sudo fuser -k 8002/tcp
cd ../../memory-module/
./Debug/memory-module ../deployment/error/memory.config