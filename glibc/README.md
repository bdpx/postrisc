
## Notes about porting to Postrisc

git clone git@github.com:bdpx/glibc.git

git remote add upstream git@github.com:bminor/glibc.git

git remote -v

git fetch upstream

git rebase -i upstream/master
