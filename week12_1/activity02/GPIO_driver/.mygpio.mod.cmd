savedcmd_mygpio.mod := printf '%s\n'   mygpio.o | awk '!x[$$0]++ { print("./"$$0) }' > mygpio.mod
