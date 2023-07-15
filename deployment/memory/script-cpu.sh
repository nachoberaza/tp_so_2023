#!/bin/bash
sudo fuser -k 8001/tcp
cd ../../cpu-module/
./Debug/cpu-module ../deployment/memory/cpu.config