
## Notes about porting to Postrisc

# update doom_generic
cd ../../../
git clone git@github.com:bdpx/postrisc_doom.git
cd ./postrisc_doom
git remote add upstream git@github.com:ozkl/doomgeneric.git
git remote -v
git fetch upstream
git rebase -i upstream/master

