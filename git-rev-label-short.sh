#!/usr/bin/env bash
set -euo pipefail
## This script was generated with 'git-rev-label -g'
## See https://gitlab.com/kyb/git-rev-label
commit=$(git rev-parse --short HEAD);
short=$commit;
count=$(git rev-list --count ${since:+--since=$since} --first-parent ${from:+$from..}HEAD);
dirty=$(git diff-index --quiet HEAD -- && git ls-files --others --error-unmatch . >/dev/null || echo dirty);
_dirty=${dirty:+-$dirty};
_DIRTY=${_dirty^^};
tag="$(git tag --list --points-at HEAD | head -1)";
branch="$(git rev-parse --abbrev-ref HEAD | sed s,^HEAD$,DETACHED,)";
refname=$(if test "$branch" == DETACHED; then echo "${tag:-DETACHED}"; else echo "$branch";fi;)
export refname count short _DIRTY
echo '#define REV_LABEL "$refname-c$count-g$short$_DIRTY"' | { 
   perl -pe'$var="[A-Za-z_][A-Za-z0-9_]+"; s,\$(?:($var)|\{($var)\}),$ENV{$1//$2}//$&,eg' 
} >rev-label.h
