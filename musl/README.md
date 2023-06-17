
## Notes about porting to Postrisc

git clone git@github.com:bdpx/musl.git

# there is no more github mirror
# git remote add upstream git@github.com:bminor/musl.git

git remote add upstream https://git.musl-libc.org/cgit/musl

git remote -v

git fetch upstream

git rebase -i upstream/master
