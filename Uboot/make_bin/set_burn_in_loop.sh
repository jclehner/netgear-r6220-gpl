#! /bin/sh
rm -fr burn_in_loop
echo -e -n "\x$1\x00" > loop
dd if=loop of=burn_in_loop bs=1 count=2
rm -fr loop
