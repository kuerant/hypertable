#!/bin/sh -e
# recursive find and replace with perl regex
[ $# -gt 1 ] || { echo "usage: $0 <from> <to> [<name_glob>]"; exit 1; }
[ "$FNR_DEBUG" ] && set -ex

git branch # kinda safeguard against non-version controled damage
findopt="-type f"
[ "$3" ] && findopt="-name $3"

find . -name .git -prune -o $findopt |
  grep -v '\.git' | xargs perl -ne "/$1/"' && print $ARGV, "\n"' | sort -u |
  xargs perl -i -pe "s/$1/$2/g"
