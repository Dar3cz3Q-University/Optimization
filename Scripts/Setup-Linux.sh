#!/bin/bash

chmod +x Vendor/Binaries/Premake/Linux/premake5
Vendor/Binaries/Premake/Linux/premake5 --cc=clang gmake2
