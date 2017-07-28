#!/bin/sh
appname="SimDSP"

dirname="$(dirname -- "$(readlink -f -- "${0}")" )"

if [ "$dirname" = "." ]; then
	dirname="$PWD/$dirname"
fi

LD_LIBRARY_PATH="${dirname}/lib"
export LD_LIBRARY_PATH

"$dirname/lib/$appname" $*
