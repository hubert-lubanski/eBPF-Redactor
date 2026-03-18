#!/bin/bash

cd $(dirname $0)
#make

run() {
	tst_exec=$1
	ok=1

	echo === Test $tst_exec ===
	rm -f tst
	if ! ./$tst_exec ; then
		echo "Exec failed"
		ok=0
	fi

	[ "$ok" == "1" ] && echo OK
}

COUNTER=1
do_check_perm() {
	echo "==== CHECK PERM $COUNTER"
	./check_perm /tmp/test && echo OK
	COUNTER=$((COUNTER + 1))
}

run simple_read
run complex_read

echo "==== FAIL CTX"
run fail_ctx

rm -f /tmp/test
touch /tmp/test
chown 0:0 /tmp/test
chmod 660 /tmp/test

do_check_perm "Same owner, 0660 mode"

chown 1000:0 /tmp/test
do_check_perm "Other user"

chown 0:1000 /tmp/test
do_check_perm "Other group"

chown 0:0 /tmp/test
chmod 664 /tmp/test
do_check_perm "Same owner, 0664 mode"
