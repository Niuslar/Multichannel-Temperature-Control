#!/bin/bash
set -eu

stm32cubeide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild "$@" \
-data /workspace/2 -import /workspace/Multichannel-Temperature-Control/ -cleanBuild 'Multichannel temperature sensor' -markerType cdt \
|| { echo "Build Failed"; exit 1; }