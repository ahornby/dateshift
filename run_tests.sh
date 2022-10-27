#!/usr/bin/env bash

# Super simple test for regression
TZ=BST diff -u <(echo "2018-01-01T01:02") <(./dateshift  -t '2018-01-01 01:02:03' -- date +"%Y-%m-%dT%H:%M")