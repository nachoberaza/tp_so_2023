#!/bin/bash
sudo fuser -k 8003/tcp
cd ../../file-system-module/
./Debug/file-system-module ../deployment/base/fileSystem.config