#!/usr/bin/env bash

#################################################################
# clone
#################################################################

# for windows: add --config core.autocrlf=false
git clone git@github.com:bdpx/llvm-project.git

git checkout main

#################################################################
# setup upstream
#################################################################

git remote add upstream git@github.com:llvm/llvm-project.git
# check upstream
git remote -v
git fetch upstream

#################################################################
# rebase
#################################################################

#Step 1: Add the remote (original repo that you forked) and call it “upstream”
#git remote add upstream https://github.com/original-repo/goes-here.git

#Step 2: Fetch all branches of remote upstream
git fetch upstream

#Step 3: Rewrite your master with upstream’s master using git rebase.
git rebase -i upstream/main

#Step 4: Push your updates to master. You may need to force the push with “--force”.
git push --force

#################################################################
# may help if somebody force-pushed upstream
#################################################################
git update-ref -d refs/remotes/upstream/main

# add executable flag (on Windows)
git update-index --chmod=+x foo.bash

