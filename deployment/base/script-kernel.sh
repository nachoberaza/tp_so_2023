#!/bin/bash
sudo fuser -k 8000/tcp
cd ../../kernel-module/
./Debug/kernel-module ../deployment/base/kernel.config