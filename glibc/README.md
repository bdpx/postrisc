
## Notes about porting to Postrisc

git clone git@github.com:bdpx/glibc.git

# there are no more github mirror
# git remote add upstream git@github.com:bminor/glibc.git

git remote add upstream https://sourceware.org/git/glibc.git

git remote -v

git fetch upstream

git rebase -i upstream/master
