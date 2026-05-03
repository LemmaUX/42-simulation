This exercise must be prepared on a Unix/Linux filesystem to set exact permissions,
ownership display, timestamps, and symlink behavior expected by Moulinette.

Suggested workflow on Linux:
1) Create files test0..test6 with required permissions.
2) Set timestamps with touch -t.
3) Make test6 a symlink to test0.
4) Run: tar -cf exo.tar *

A helper script is provided in build_exo.sh.
