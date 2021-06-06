#!/bin/bash
rm -rf release
mkdir -p release

cp -rf MyAudioEffect addon.json LICENSE release/

mv release score-node-my-audio-effect
7z a score-node-my-audio-effect.zip score-node-my-audio-effect
