# New in 1.2

Updated libtool/automake build scripts to latest versions

Applied fix DST from  Pavel Kankovsky and added a test for it.  This now works:
 
```shell
./install/bin/dateshift  -t '2018-01-01 01:02:03' -- date
Mon  1 Jan 01:02:03 GMT 2018
```

# New in 1.1

Added support for clock_gettime interception
Changed command line handling to better support executing a command directly
Updated libtool/automake build scripts to latest versions

# New in 1.0

First released version.
