#!/usr/bin/env bash
# set -euo pipefail

if test -z "$1"
then OUT=byte_vector.hh
elif test -d "$1" 
then OUT="$1"/byte_vector.hh
else OUT="$1"
fi

rm_once(){
   sed -E '/^#pragma once/'d "$@"
}

cat >"$OUT" <<END
#pragma once

/////////////  byte_vector.hpp  /////////////////
$(rm_once byte_vector.hpp)

/////////////  byte_range_ascii.hpp  /////////////////
$(rm_once byte_range_ascii.hpp)

/////////////  byte_range_view.hpp  /////////////////
$(rm_once byte_range_view.hpp)

/////////////  byte_range_hex.hpp  /////////////////
$(rm_once byte_range_hex.hpp)
END
