#!/usr/bin/env bash

# Super simple test for regression
diff -u <(echo "Mon  1 Jan 01:02:03 GMT 2018") <(./install/bin/dateshift  -t '2018-01-01 01:02:03' -- date)