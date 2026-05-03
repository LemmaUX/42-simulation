#!/bin/sh

rm -f test0 test1 test2 test3 test4 test5 test6 exo.tar

touch test0 test1 test2 test3 test4 test5
ln -s test0 test6

chmod 715 test0
chmod 714 test1
chmod 504 test2
chmod 404 test3
chmod 641 test4
chmod 404 test5
chmod 777 test6

# Example timestamps from the subject, adjust year automatically.
touch -t 202406012047 test0
touch -t 202406012146 test1
touch -t 202406012245 test2
touch -t 202406012344 test3
touch -t 202406012343 test4
touch -t 202406012344 test5
touch -h -t 202406012220 test6

tar -cf exo.tar *
