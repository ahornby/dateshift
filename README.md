# Dateshift

Dateshift is a tool for shifting date/time. In particular it allows a
process or set of process to run with a different clock setting,
_without_ having to change the system clock.

This is useful because changing the system clock is requires root
privilege and is inconvenient on shared machines.

Some example uses:

* Use dateshift to test your finance package on 29th Feb

* Use dateshift to find out what happens when unix time wraps around in 2037.

* Use dateshift to test your new clock program!

See the man page for user documentation.

## Example usage

```shell
$ ./install/bin/dateshift  -t '2018-01-01 01:02:03' -- date
Mon  1 Jan 01:02:03 GMT 2018
```

## Copyright

Dateshift is Copyright 2002,2008,2022 Alex Hornby <alex@hornby.org.uk> under
the terms of the GNU General Public License version 2 or later.

## Disclaimer

This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
